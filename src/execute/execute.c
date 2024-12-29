#include "execute.h"
#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "execute/t_pid_list/t_pid_list.h"
#include "execute/process/process.h"
#include "parse/t_command/t_command.h"
#include "io/t_redir_list/t_redir_list.h"
#include "t_pid_list/t_pid_list.h"
#include "word/t_word_list/t_word_list.h"
#include "word/expansions/expand.h"
#include "signal/signal.h"
#include "libft/ft_io.h"

#include "./builtin/builtin.h"

#include <stdio.h>
#include <stdlib.h>
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

// either simple or subshell or maybe builtin
t_launch_result launch_pipeline_inner(t_state* state, t_command command, t_io io, int fd_to_close) {
	assert(command.type == CMD_SIMPLE || command.type == CMD_SUBSHELL);

	t_error err;

	if (command.type == CMD_SIMPLE) {
		t_expansion_variables vars = (t_expansion_variables){state->env, state->last_status};
		err = variable_expand_words(vars, &command.simple->words);
		if (err != NO_ERROR)
			return (t_launch_result){.error = err, .pids = NULL};

		if (/* command.simple->words && */ is_builtin_command(command.simple))
			return (launch_cmd_in_subshell(state, command, io, fd_to_close));
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

		if (pipe(pipe_fd) < 0)
		{
			// BAD: should close pipe related file descriptors where necessary
			kill_pipeline(state, pids_to_wait);
			pidl_clear(&pids_to_wait);
			return (t_launch_result) {.pids = NULL , .error = E_PIPE};
		}

		t_io current_io = io_new(ends.input, pipe_fd[WRITE]);
		ends.input = pipe_fd[READ];

		t_launch_result launch_result = launch_pipeline_inner(state, current.pipeline->first,
													  current_io, pipe_fd[READ]);
		if (launch_result.error != NO_ERROR)
		{
			// BAD: should close pipe related file descriptors where necessary
			kill_pipeline(state, pids_to_wait);
			pidl_clear(&pids_to_wait);
			return (t_launch_result) {.pids = NULL , .error = launch_result.error};
		}

		io_close(current_io);

		pidl_push_back_link(&pids_to_wait, launch_result.pids); // bad might oom

		current = current.pipeline->second;
	}
	t_launch_result last = launch_pipeline_inner(state, current, ends, CLOSE_NOTHING); // bad, handle error
	io_close(ends);

	pidl_push_back_link(&pids_to_wait, last.pids); // bad may oom
	return (t_launch_result){.error = NO_ERROR, .pids = pids_to_wait};
}

#define COMMAND_NOT_FOUND_EXIT_CODE 127
#define NOT_EXECUTABLE_EXIT_CODE 126

static bool file_is_directory(const char *command_path)
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
	if (error == ENOENT)
		state->last_status = COMMAND_NOT_FOUND_EXIT_CODE;
	else
		state->last_status = NOT_EXECUTABLE_EXIT_CODE;

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
	t_pid_list* pids = NULL;
	bool in_child;

	err = fork_and_push_pid(&in_child, &pids);
	if (err != NO_ERROR)
		return (t_launch_result){.error = err, .pids = NULL};

	if (!in_child)
		return (t_launch_result){.error = NO_ERROR, .pids = pids};

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("minishell: do_piping: dup2");

	err = apply_redirections(state, simple->redirections);
	if (err != NO_ERROR) /* exit with status EXIT_FAILURE after logging error (execute_cmd.c:797) */
		graceful_exit_from_child(EXIT_FAILURE);

#if 1 // exit here if command is null!
	if (!simple->words)
		graceful_exit_from_child(EXIT_SUCCESS);
#endif

	char *command_path;
	err = path_expanded_word(state->env, simple->words->contents, &command_path);
	if (err == E_COMMAND_NOT_FOUND && command_path == NULL)
	{
		log_command_not_found(simple->words->contents);
		graceful_exit_from_child(COMMAND_NOT_FOUND_EXIT_CODE);
	}
	if (err != NO_ERROR)
		graceful_exit_from_child(EXIT_FAILURE); // bad, only possible error here is OOM, handle accordingly

	// exit here instead of executing if last_signal is SIGINT
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

		if (/* command.simple->words && */ is_builtin_command(command.simple))
		{
			int io_backup[2]; // TODO: make sure all files are properly closed and messages are printed in case of errors

			if (save_standard_input_and_output(io_backup) != NO_ERROR)
				return (t_command_result){.error = NO_ERROR, .status_code = EXIT_FAILURE}; // bad: should maybe notify
			err = apply_redirections(state, command.simple->redirections);
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

			int exit_status;
			err = wait_for_process(state, launch_res.pids->pid, &exit_status);
			if (err != NO_ERROR)
			{
				exit_status = EXIT_FAILURE;
				perror("minishell: wait_for_pipeline");
			}
			pidl_clear(&launch_res.pids);
			res = (t_command_result){.error = NO_ERROR, .status_code = exit_status};
		}
	}
	else if (command.type == CMD_PIPELINE)
	{
		t_launch_result launch_res;
		launch_res = launch_pipeline(state, command.pipeline, io_default());
		// E_PIPE -> `last_status = EXIT_FAILURE | 128` (execute_cmd.c:2522 and sig.c:418)
		// E_FORK -> `last_status = EXIT_FAILURE | (EX_NOEXEC = 126)` (execute_cmd.c:4443 and jobs.c:4443)
		// E_OOM -> propagate
		assert(launch_res.error == NO_ERROR); // bad, should handle launch error gracefully

		assert(launch_res.pids != NULL);
		int last_exit_status;
		err = wait_for_pipeline(state, launch_res.pids, &last_exit_status);
		// the only errors `waitpid` can return to us are EINVAL and ECHILD, the
		// first being a programming error and the other one signifying that the
		// process was somehow never launched or does not belong to us anymore.
		// meaning they are effectively unreachable, only warn for them in this case
		if (err != NO_ERROR)
		{
			last_exit_status = EXIT_FAILURE;
			perror("minishell: wait_for_pipeline");
		}
		pidl_clear(&launch_res.pids);
		res = (t_command_result){.error = NO_ERROR, .status_code = last_exit_status};
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
