#include "execute.h"
#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "execute/process/process.h"
#include "execute/t_env/t_env_internals.h"
#include "parse/t_command/t_command.h"
#include "io/t_redir_list/t_redir_list.h"
#include "word/t_word_list/t_word_list.h"
#include "word/expansions/expand.h"
#include "signal/signal.h"
#include "libft/ft_io.h"

#include "./builtin/builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h> // bad

#define PIPE_READ 0
#define PIPE_WRITE 1

void shell_cleanup(t_state *state); // BAD: this should be #include "shell.h"

_Noreturn
static void cleanup_and_die(t_state *state, int with_status)
{
	shell_cleanup(state);
	exit(with_status);
}

// either simple or subshell or maybe builtin
t_error launch_pipeline_inner(t_state* state, t_command command, t_io io, int fd_to_close) {
	assert(command.type == CMD_SIMPLE || command.type == CMD_SUBSHELL);

	t_error err;

	if (command.type == CMD_SIMPLE) {
		t_expansion_variables vars = (t_expansion_variables){state->env, state->last_status};
		err = variable_expand_words(vars, &command.simple->words);
		if (err != NO_ERROR)
			return err;

		if (/* command.simple->words && */ is_builtin_command(command.simple))
			return (launch_cmd_in_subshell(state, command, io, fd_to_close));
		return launch_simple_command(state, command.simple, io, fd_to_close);
	}
	else // subshell
		return launch_subshell(state, command.subshell, io, fd_to_close);
}

t_error launch_pipeline(t_state *state, t_pipeline *pipeline, t_io ends)
{
	t_command current;
	t_error err;

	current = command_from_pipeline(pipeline);
	while (current.type == CMD_PIPELINE)
	{
		int pipe_fd[2];

		if (pipe(pipe_fd) < 0)
		{
			// BAD: should close pipe related file descriptors where necessary
			kill_pipeline(state, state->our_children);
			pidl_clear(&state->our_children);
			return E_PIPE;
		}

		t_io current_io = io_new(ends.input, pipe_fd[PIPE_WRITE]);
		ends.input = pipe_fd[PIPE_READ];

		err = launch_pipeline_inner(state, current.pipeline->first,
													  current_io, pipe_fd[PIPE_READ]);
		if (err != NO_ERROR)
		{
			// BAD: should close pipe related file descriptors where necessary
			kill_pipeline(state, state->our_children);
			pidl_clear(&state->our_children);
			return err;
		}

		io_close(current_io);

		current = current.pipeline->second;
	}
	err = launch_pipeline_inner(state, current, ends, CLOSE_NOTHING); // bad, handle error
	io_close(ends);

	return NO_ERROR;
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

static void free_null_terminated_str_array(char *arr[])
{
	size_t i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i] != NULL)
		free(arr[i]);
	free(arr);
}

_Noreturn
static void execve_and_exit(t_state* state, char* command_path, char **argv, char **envp)
{
	execve(command_path, argv, envp);
	int error = errno;
	free_null_terminated_str_array(argv);
	free_null_terminated_str_array(envp);

	if (error == ENOENT) // (execute_cmd.c:5967)
		state->last_status = COMMAND_NOT_FOUND_EXIT_CODE;
	else
		state->last_status = NOT_EXECUTABLE_EXIT_CODE;

	if (file_is_directory(command_path)) { // maybe we should check S_ISREG aswell
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command_path, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		free(command_path);
		cleanup_and_die(state, state->last_status);
	}

	perror("minishell: execve");
	free(command_path);
	cleanup_and_die(state, state->last_status);
}

static void log_command_not_found(const char *pathname) // bad? input may be split by other processes writing to stderr
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(pathname, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

t_error launch_simple_command(t_state *state, t_simple *simple, t_io io, int fd_to_close)
{
	t_error err;
	bool in_child;

	err = fork_and_push_pid(&in_child, &state->our_children);
	if (err != NO_ERROR)
		return err;

	if (!in_child)
		return NO_ERROR;
	else
		pidl_clear(&state->our_children);

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("minishell: do_piping: dup2");

	err = apply_redirections(state, simple->redirections);
	if (err != NO_ERROR) // bad: calls write many times while trying to write an entire line
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(error_repr(err), STDERR_FILENO);
		if (is_syscall_related(err))
		{
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putstr_fd(strerror(errno), STDERR_FILENO);
		}
		ft_putchar_fd('\n', STDERR_FILENO);
		cleanup_and_die(state, EXIT_FAILURE);
	}

	if (!simple->words) // exit here if command is null
		cleanup_and_die(state, EXIT_SUCCESS);

	char *command_path;
	err = path_expanded_word(state->env, simple->words->contents, &command_path);
	if (err == E_COMMAND_NOT_FOUND && command_path == NULL)
	{
		log_command_not_found(simple->words->contents);
		cleanup_and_die(state, COMMAND_NOT_FOUND_EXIT_CODE);
	}
	if (err != NO_ERROR)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(error_repr(err), STDERR_FILENO);
		if (is_syscall_related(err))
		{
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putstr_fd(strerror(errno), STDERR_FILENO);
		}
		ft_putchar_fd('\n', STDERR_FILENO);
		cleanup_and_die(state, EXIT_FAILURE);
	}

	// exit here instead of executing if last_signal is SIGINT
	reset_signal_handlers();

	char** argv = wl_into_word_array(&simple->words);
	if (!argv)
	{
		ft_putstr_fd("minishell: Out of memory\n", STDERR_FILENO);
		cleanup_and_die(state, EXIT_FAILURE);
	}
	char** envp = env_make_envp(state->env);
	if (!envp)
	{
		free_null_terminated_str_array(argv);
		ft_putstr_fd("minishell: Out of memory\n", STDERR_FILENO);
		cleanup_and_die(state, EXIT_FAILURE);
	}

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
			// E_FORK -> `last_status = (EX_NOEXEC = 126) | 128` (jobs.c:2210 and sig.c:418)
			err = launch_simple_command(state, command.simple, io_default(), CLOSE_NOTHING);
			if (err != NO_ERROR)
				return (t_command_result){.error = err};
			assert(state->our_children != NULL);

			int exit_status;
			err = wait_for_process(state, state->our_children->pid, &exit_status);
			if (err != NO_ERROR)
			{
				exit_status = EXIT_FAILURE;
				perror("minishell: wait_for_pipeline");
			}
			pidl_clear(&state->our_children);
			res = (t_command_result){.error = NO_ERROR, .status_code = exit_status};
		}
	}
	else if (command.type == CMD_PIPELINE)
	{
		err = launch_pipeline(state, command.pipeline, io_default());
		// E_PIPE -> `last_status = EXIT_FAILURE | 128` (execute_cmd.c:2495 and sig.c:418)
		// E_FORK -> `last_status = (EX_NOEXEC = 126) | 128` (jobs.c:2210 and sig.c:418)
		// E_OOM -> propagate
		if (err != NO_ERROR)
			return (t_command_result){.error = err};
		assert(state->our_children != NULL);

		int last_exit_status;
		err = wait_for_pipeline(state, state->our_children, &last_exit_status);
		// the only errors `waitpid` can return to us are EINVAL and ECHILD, the
		// first being a programming error and the other one signifying that the
		// process was somehow never launched or does not belong to us anymore.
		// meaning they are effectively unreachable, only warn for them in this case
		if (err != NO_ERROR)
		{
			last_exit_status = EXIT_FAILURE;
			perror("minishell: wait_for_pipeline");
		}
		pidl_clear(&state->our_children);
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
