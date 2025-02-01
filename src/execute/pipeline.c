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
#include <errno.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

#define EXIT_FAILURE 1
#define EX_NOEXEC 126

t_command_result	execute_pipeline(t_state *state, t_pipeline *pipeline)
{
	t_error	err;
	int		last_exit_status;

	err = launch_pipeline(state, pipeline, io_default());
	if (err == E_PIPE)
		return (report_syscall_error("pipe"), command_ok(EXIT_FAILURE | 128));
	else if (err == E_FORK)
		return (report_syscall_error("fork"), command_ok(EX_NOEXEC | 128));
	else if (err != NO_ERROR)
		return (command_err(err));
	assert(state->our_children != NULL);
	err = wait_for_pipeline(state, state->our_children, &last_exit_status);
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
	else if (command.type == CMD_SUBSHELL)
		return (launch_subshell(state, command.subshell, io, fd_to_close));
	else
		return (E_UNREACHABLE);
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
	err = launch_pipeline_inner(state, current, ends, CLOSE_NOTHING);
	if (err != NO_ERROR)
		return (pipeline_cleanup(state, ends, CLOSE_NOTHING), err);
	io_close(ends);
	return (NO_ERROR);
}
