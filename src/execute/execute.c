#include "execute.h"
#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "execute/t_pid_list/t_pid_list.h"
#include "parse/t_command/t_command.h"
#include "io/t_redir_list/t_redir_list.h"
#include "word/t_word_list/t_word_list.h"
#include "word/expansions/expand.h"
#include "signal/signal.h"
#include "libft/ft_io.h"

#include "./builtin/builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h> // bad

#define READ 0
#define WRITE 1

// ? perform expansion on all words -> malloc
// ? expand_path on first word -> malloc,access
// ? generate argv, envp -> malloc
// ? fork
//  ---->   parent
//   |      return
//   |
//   |----> child
//           ? apply t_io redirections -> dup2
//		     ? apply all simple->redirections -> open,dup2 NOT close
//		     ? execute command execve
//           ---->   fails
//            |      unwind all local allocations
//            |      close local fds
//            |      signal `must_exit`
//            |
//            |----> succeeds

_Noreturn
static void graceful_exit_from_child(int with_status) // bad dummy
{
	exit(with_status); // bad, should clean up all allocations before exiting from child process
}

int wait_pipeline(t_pid_list* pids) // bad, should handle EINTR
{
	t_pid_list* current = pids;
	int status;

	while (current)
	{
		waitpid(current->pid, &status, 0); // hack, should not wait sequentially?
		current = current->next;
	}
	return (status);
}

// either simple or subshell or maybe builtin
t_launch_result launch_pipeline_inner(t_state* state, t_command command, t_io io, int fd_to_close) {
	assert(command.type == CMD_SIMPLE || command.type == CMD_SUBSHELL);

	t_error err;

	if (command.type == CMD_SIMPLE) {
		t_expansion_variables vars = (t_expansion_variables){state->env, state->last_status};
		err = variable_expand_words(vars, &command.simple->words);
		if (err != NO_ERROR)
			return (t_launch_result){.error = err, .pids = NULL};

		if (is_builtin_command(command.simple))
		{
			t_command subshell = command_new_subshell(command, NULL);
			if (!subshell.subshell)
				return (t_launch_result){.error = E_OOM, .pids = NULL};
			t_launch_result res = launch_subshell(state, subshell.subshell, io, fd_to_close);
			free(subshell.subshell);
			return res;
		}
		return launch_simple_command(state, command.simple, io, fd_to_close);
	}
	else // subshell
		return launch_subshell(state, command.subshell, io, fd_to_close);
}

t_launch_result launch_pipeline(t_state *state, t_pipeline *pipeline, t_io ends)
{
	t_command current;
	t_pid_list* pids_to_wait = NULL;

	current = command_from_pipeline(pipeline);
	while (current.type == CMD_PIPELINE)
	{
		int pipe_fd[2];

		// bad, should handle pipe error by killing all jobs thus far and setting
		// last status to `EXIT_FAILURE | 128` (execute_cmd.c:2522 and sig.c:418)
		pipe(pipe_fd);

		t_io current_io = io_new(ends.input, pipe_fd[WRITE]);
		ends.input = pipe_fd[READ];

		// bad, should handle fork error by killing all jobs thus far and setting
		// last status to `EXIT_FAILURE | (EX_NOEXEC = 126)` (execute_cmd.c:4443
		// and jobs.c:4443)
		t_launch_result launch_result = launch_pipeline_inner(state, current.pipeline->first,
													  current_io, pipe_fd[READ]);

		io_close(current_io);

		pidl_push_back_link(&pids_to_wait, launch_result.pids); // bad might oom

		current = current.pipeline->second;
	}
	t_launch_result last = launch_pipeline_inner(state, current, ends, CLOSE_NOTHING);
	io_close(ends);

	pidl_push_back_link(&pids_to_wait, last.pids); // bad may oom
	return (t_launch_result){.error = NO_ERROR, .pids = pids_to_wait};
}

#define COMMAND_NOT_FOUND_EXIT_CODE 127
#define NOT_EXECUTABLE_EXIT_CODE 126

bool file_is_directory(const char *command_path)
{
	struct stat command_stats;
	if (stat(command_path, &command_stats) < 0)
		return false;
	return (S_ISDIR(command_stats.st_mode));
}

_Noreturn
static void execve_and_exit(t_state* state, const char* command_path, char **argv, char **envp)
{
	execve(command_path, argv, envp);
	int error = errno;

	// ???? (execute_cmd.c:5967)
	state->last_status = error == ENOENT ? COMMAND_NOT_FOUND_EXIT_CODE : NOT_EXECUTABLE_EXIT_CODE;

	if (file_is_directory(command_path)) { // maybe we should check S_ISREG aswell
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command_path, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		exit(state->last_status); // bad no cleanup
	}

	perror("minishell: execve");
	exit(state->last_status); // bad no cleanup
}

static void log_command_not_found(const char *pathname) // bad? input may be split by other processes writing to stderr
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(pathname, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

t_launch_result launch_simple_command(t_state *state, t_simple *simple, t_io io, int fd_to_close)
{
	t_error err;
	t_pid_list* pids = pidl_new(0);
	if (pids == NULL)
	{
		free(pids);
		return (t_launch_result){.error = E_OOM, .pids = NULL};
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		free(pids);
		return (t_launch_result){.error = E_FORK, .pids = NULL};
	}
	if (pid != 0)
	{
		pids->pid = pid;
		return (t_launch_result){.error = NO_ERROR, .pids = pids};
	}

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("dup2");

	err = apply_redirections(simple->redirections);
	if (err != NO_ERROR) /* exit with status EXIT_FAILURE after logging error (execute_cmd.c:797) */
		graceful_exit_from_child(EXIT_FAILURE);

	char *command_path;
	err = path_expanded_word(state->env, simple->words->contents, &command_path);
	if (err == E_COMMAND_NOT_FOUND && command_path == NULL)
	{
		log_command_not_found(simple->words->contents);
		graceful_exit_from_child(COMMAND_NOT_FOUND_EXIT_CODE);
	}
	if (err != NO_ERROR)
		graceful_exit_from_child(EXIT_FAILURE); // bad, only possible error here is OOM, handle accordingly

	// temporarily? where exactly in the code should signal handlers be reset?
	// what happens if we've caught one up until this point? exit(128 + signal)?
	// what about inside of built-in functions?
	reset_signal_handlers();

	char** argv = wl_into_word_array(&simple->words);
	char** envp = env_make_envp(state->env);

	assert(argv && envp);

	execve_and_exit(state, command_path, argv, envp);
}

t_error save_standard_input_and_output(int save[2])
{
	int in;
	int out;

	in = dup(STDIN_FILENO);
	if (in == -1)
		return (E_DUP2);
	out = dup(STDOUT_FILENO);
	if (out == -1)
		return (close(out), E_DUP2);
	save[0] = in;
	save[1] = out;
	return (NO_ERROR);
}

t_error restore_standard_input_and_output(int save[2])
{
	int in;
	int out;

	in = save[0];
	out = save[1];
	if (dup2(in, STDIN_FILENO) < 0)
		return (E_DUP2);
	if (dup2(out, STDOUT_FILENO) < 0)
		return (E_DUP2);
	close(in);
	close(out);
	return (NO_ERROR);
}

t_command_result execute_command(t_state *state, t_command command) {

	t_command_result res;
	t_error err;

	if (command.type == CMD_SIMPLE)
	{
		t_expansion_variables vars = (t_expansion_variables){state->env, state->last_status};
		err = variable_expand_words(vars, &command.simple->words);
		if (err != NO_ERROR)
			return (t_command_result){.error = err};

		if (is_builtin_command(command.simple))
		{
			int io_backup[2]; // TODO: make sure all files are properly closed and messages are printed in case of errors

			if (save_standard_input_and_output(io_backup) != NO_ERROR)
				return (t_command_result){.error = NO_ERROR, .status_code = EXIT_FAILURE}; // bad: should maybe notify
			err = apply_redirections(command.simple->redirections);
			if (err != NO_ERROR)
				return (t_command_result){.error = NO_ERROR, .status_code = EXIT_FAILURE}; // bad: should maybe notify
			res = execute_builtin(state, command.simple);
			if (restore_standard_input_and_output(io_backup) != NO_ERROR)
				return (t_command_result){.error = NO_ERROR, .status_code = EXIT_FAILURE}; // bad: should maybe notify
		}
		else 
		{
			t_launch_result launch_res = launch_simple_command(state, command.simple, io_default(), CLOSE_NOTHING);
			assert(launch_res.error == NO_ERROR); // bad, should handle launch error gracefully

			int status;
			int options = 0;
			assert(launch_res.pids != NULL);
			waitpid(launch_res.pids->pid, &status, options); // bad, `waitpid` errors should be handled
			res = (t_command_result){.error = NO_ERROR, .status_code = WEXITSTATUS(status)}; // bad, might err; and make sure waitpid worked before checking WEXITSTATUS
		}
	}
	else if (command.type == CMD_PIPELINE)
	{
		t_launch_result launch_res;
		launch_res = launch_pipeline(state, command.pipeline, io_default());
		assert(launch_res.error == NO_ERROR); // bad, should handle launch error gracefully

		int status = wait_pipeline(launch_res.pids);
		res = (t_command_result){.error = NO_ERROR, .status_code = status};
	}
	else if (command.type == CMD_CONDITIONAL)
		res = execute_conditional(state, command.conditional);
	else if (command.type == CMD_SUBSHELL)
		res = execute_subshell(state, command.subshell);
	else
		assert(!"unknown command type");

	if (res.error != NO_ERROR)
		printf("err : %d\n",res.error);

	return res;
}
