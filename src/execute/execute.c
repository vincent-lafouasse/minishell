#include "execute.h"

#include "error/t_error.h"
#include "shell/shell.h"
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

// either simple or subshell or maybe builtin
static t_error launch_pipeline_inner(t_state* state, t_command command, t_io io, int fd_to_close) {
	assert(command.type == CMD_SIMPLE || command.type == CMD_SUBSHELL);

	t_expansion_variables vars;
	t_error err;

	if (command.type == CMD_SIMPLE) {
		vars = (t_expansion_variables){state->env, state->last_status};
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
			io_close(ends);
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
			io_close(io_new(pipe_fd[PIPE_READ], pipe_fd[PIPE_WRITE]));
			io_close(current_io);
			kill_pipeline(state, state->our_children);
			pidl_clear(&state->our_children);
			return err;
		}

		io_close(current_io);

		current = current.pipeline->second;
	}
	err = launch_pipeline_inner(state, current, ends, CLOSE_NOTHING); // bad, handle error
	if (err != NO_ERROR)
	{
		close(ends.input);
		kill_pipeline(state, state->our_children);
		pidl_clear(&state->our_children);
		return err;
	}
	io_close(ends);

	return NO_ERROR;
}

t_error save_stdin_stdout(int save[2])
{
	int in;
	int out;

	in = dup(STDIN_FILENO);
	if (in == -1)
		return (E_DUP2);
	out = dup(STDOUT_FILENO);
	if (out == -1)
		return (close(in), E_DUP2);
	save[0] = in;
	save[1] = out;
	return (NO_ERROR);
}

t_error restore_stdin_stdout(int save[2])
{
	int in;
	int out;

	in = save[0];
	out = save[1];
	if (dup2(in, STDIN_FILENO) < 0)
		return (E_DUP2);
	if (dup2(out, STDOUT_FILENO) < 0)
		return (E_DUP2);
	return (NO_ERROR);
}

void close_fd_pair(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

t_command_result	do_redirs_and_execute_builtin(t_state *state, t_simple *builtin)
{
	t_command_result res;
	int io_backup[2];
	t_error err;

	err = save_stdin_stdout(io_backup);
	if (err != NO_ERROR)
	{
		report_syscall_error(error_repr(err));
		return command_ok(EXIT_FAILURE);
	}
	err = apply_redirections(state, builtin->redirections);
	if (err != NO_ERROR)
	{
		if (is_syscall_related(err))
			report_syscall_error("apply_redirections");
		else
			report_t_error("apply_redirections", err);
		close_fd_pair(io_backup);
		return command_ok(EXIT_FAILURE);
	}
	res = execute_builtin(state, builtin);
	err = restore_stdin_stdout(io_backup);
	close_fd_pair(io_backup);
	if (err != NO_ERROR && res.error == NO_ERROR)
	{
		report_syscall_error(error_repr(err));
		return command_ok(EXIT_FAILURE);
	}
	return res;
}

t_command_result execute_simple(t_state *state, t_simple *simple)
{
	t_error err;

	err = launch_simple_command(state, simple, io_default(), CLOSE_NOTHING);
	// E_FORK -> `last_status = (EX_NOEXEC = 126) | 128` (jobs.c:2210 and sig.c:418)
	if (err == E_FORK)
		return report_syscall_error("fork"), command_ok(126 | 128);
	else if (err != NO_ERROR)
		return command_err(err);
	assert(state->our_children != NULL);

	int exit_status;
	err = wait_for_process(state, state->our_children->pid, &exit_status);
	if (err != NO_ERROR)
	{
		exit_status = EXIT_FAILURE;
		perror("minishell: wait_for_pipeline");
	}
	pidl_clear(&state->our_children);
	return (command_ok(exit_status));
}

t_command_result execute_simple_or_builtin(t_state *state, t_simple *simple)
{
	t_expansion_variables vars;
	t_error err;

	vars = (t_expansion_variables){state->env, state->last_status};
	err = variable_expand_words(vars, &simple->words);
	if (err != NO_ERROR)
		return command_err(err);

	if (/* simple->words && */ is_builtin_command(simple))
		return (do_redirs_and_execute_builtin(state, simple));
	else
		return (execute_simple(state, simple));
}

t_command_result execute_pipeline(t_state *state, t_pipeline *pipeline)
{
	t_error err;

	err = launch_pipeline(state, pipeline, io_default());
	// E_PIPE -> `last_status = EXIT_FAILURE | 128` (execute_cmd.c:2495 and sig.c:418)
	// E_FORK -> `last_status = (EX_NOEXEC = 126) | 128` (jobs.c:2210 and sig.c:418)
	// E_OOM -> propagate
	if (err == E_PIPE)
		return report_syscall_error("pipe"), command_ok(1 | 128);
	if (err == E_FORK)
		return report_syscall_error("fork"), command_ok(126 | 128);
	if (err != NO_ERROR)
		return command_err(err);
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
	return (command_ok(last_exit_status));
}

t_command_result execute_command(t_state *state, t_command command) {

	t_command_result res;

	assert(state->our_children == NULL);
	if (command.type == CMD_SIMPLE)
		res = execute_simple_or_builtin(state, command.simple);
	else if (command.type == CMD_PIPELINE)
		res = execute_pipeline(state, command.pipeline);
	else if (command.type == CMD_CONDITIONAL)
		res = execute_conditional(state, command.conditional);
	else if (command.type == CMD_SUBSHELL)
		res = execute_subshell(state, command.subshell);
	else
		res = command_err(E_UNREACHABLE);

	return res;
}
