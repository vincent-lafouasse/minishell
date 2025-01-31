/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:47:07 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:53:18 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./builtin/builtin.h"
#include "error/t_error.h"
#include "execute.h"
#include "execute/process/process.h"
#include "parse/t_command/t_command.h"
#include "redirection/t_io/t_io.h"
#include "shell/shell.h"
#include "word/expansions/expand.h"
#include <assert.h> // bad
#include <errno.h>  // bad

#define PIPE_READ 0
#define PIPE_WRITE 1

t_command_result	execute_pipeline(t_state *state, t_pipeline *pipeline)
{
	t_error	err;
	int		last_exit_status;

	err = launch_pipeline(state, pipeline, io_default());
	// E_PIPE -> `last_status = EXIT_FAILURE | 128` (execute_cmd.c:2495 and sig.c:418)
	// E_FORK -> `last_status = (EX_NOEXEC = 126) | 128` (jobs.c:2210 and sig.c:418)
	// E_OOM -> propagate
	if (err == E_PIPE)
		return (report_syscall_error("pipe"), command_ok(1 | 128));
	else if (err == E_FORK)
		return (report_syscall_error("fork"), command_ok(126 | 128));
	else if (err != NO_ERROR)
		return (command_err(err));
	assert(state->our_children != NULL);
	err = wait_for_pipeline(state, state->our_children, &last_exit_status);
	// the only errors `waitpid` can return to us are EINVAL and ECHILD, the
	// first being a programming error and the other one signifying that the
	// process was somehow never launched or does not belong to us anymore.
	// meaning they are effectively unreachable, only warn for them in this case
	if (err != NO_ERROR)
	{
		last_exit_status = EXIT_FAILURE;
		report_syscall_error("wait_for_pipeline");
	}
	pidl_clear(&state->our_children);
	return (command_ok(last_exit_status));
}

// either simple or subshell or maybe builtin
static t_error	launch_pipeline_inner(t_state *state, t_command command,
		t_io io, int fd_to_close)
{
	t_expansion_variables	vars;
	t_error					err;

	assert(command.type == CMD_SIMPLE || command.type == CMD_SUBSHELL);
	if (command.type == CMD_SIMPLE)
	{
		vars = (t_expansion_variables){state->env, state->last_status};
		err = variable_expand_words(vars, &command.simple->words);
		if (err != NO_ERROR)
			return (err);
		if (command.simple->words && is_builtin_command(command.simple))
			return (launch_cmd_in_subshell(state, command, io, fd_to_close));
		return (launch_simple_command(state, command.simple, io, fd_to_close));
	}
	else // subshell
		return (launch_subshell(state, command.subshell, io, fd_to_close));
}

static void	pipeline_cleanup(t_state *state, t_io pipe, int prev_pipe_read)
{
	int	error_save;

	error_save = errno;
	if (prev_pipe_read != CLOSE_NOTHING)
		close(prev_pipe_read);
	io_close(pipe);
	kill_pipeline(state, state->our_children);
	pidl_clear(&state->our_children);
	errno = error_save;
}

t_error	launch_pipeline(t_state *state, t_pipeline *pipeline, t_io ends)
{
	t_command	current;
	t_error		err;
	int			pipe_fd[2];
	t_io		current_io;

	current = command_from_pipeline(pipeline);
	while (current.type == CMD_PIPELINE)
	{
		if (pipe(pipe_fd) < 0)
			return (pipeline_cleanup(state, ends, CLOSE_NOTHING), E_PIPE);
		current_io = io_new(ends.input, pipe_fd[PIPE_WRITE]);
		ends.input = pipe_fd[PIPE_READ];
		err = launch_pipeline_inner(state, current.pipeline->first, current_io,
				pipe_fd[PIPE_READ]);
		if (err != NO_ERROR)
			return (pipeline_cleanup(state, current_io, ends.input), err);
		io_close(current_io);
		current = current.pipeline->second;
	}
	err = launch_pipeline_inner(state, current, ends, CLOSE_NOTHING); // bad, handle error
	if (err != NO_ERROR) return (pipeline_cleanup(state, ends,
			CLOSE_NOTHING), err);
	io_close(ends);
	return (NO_ERROR);
}
