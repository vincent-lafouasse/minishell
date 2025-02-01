/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:43:26 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:43:34 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./builtin/builtin.h"
#include "error/t_error.h"
#include "execute.h"
#include "execute/process/process.h"
#include "shell/shell.h"
#include "word/expansions/expand.h"
#include <assert.h> // bad
#include <sys/stat.h>
#include <unistd.h>

static t_error			save_stdin_stdout(int save[2]);
static t_error			restore_stdin_stdout(int save[2]);
static t_command_result	do_redirs_and_execute_builtin(t_state *state,
							t_simple *builtin);

t_command_result	execute_simple_or_builtin(t_state *state, t_simple *simple)
{
	t_expansion_variables	vars;
	t_error					err;

	vars = (t_expansion_variables){state->env, state->last_status};
	err = variable_expand_words(vars, &simple->words);
	if (err != NO_ERROR)
		return (command_err(err));
	if (simple->words && is_builtin_command(simple))
		return (do_redirs_and_execute_builtin(state, simple));
	else
		return (execute_simple(state, simple));
}

// E_FORK -> `last_status = (EX_NOEXEC = 126) | 128` (jobs.c:2210 and sig.c:418)
t_command_result	execute_simple(t_state *state, t_simple *simple)
{
	t_error	err;
	int		exit_status;

	err = launch_simple_command(state, simple, io_default(), CLOSE_NOTHING);
	if (err == E_FORK)
		return (report_syscall_error("fork"), command_ok(126 | 128));
	else if (err != NO_ERROR)
		return (command_err(err));
	assert(state->our_children != NULL);
	err = wait_for_process(state, state->our_children->pid, &exit_status);
	if (err != NO_ERROR)
	{
		exit_status = EXIT_FAILURE;
		report_syscall_error("wait_for_process");
	}
	pidl_clear(&state->our_children);
	return (command_ok(exit_status));
}

static t_command_result	do_redirs_and_execute_builtin(t_state *state,
		t_simple *builtin)
{
	t_command_result	res;
	int					io_backup[2];
	t_error				err;

	err = save_stdin_stdout(io_backup);
	if (err != NO_ERROR)
	{
		report_syscall_error(error_repr(err));
		return (command_ok(EXIT_FAILURE));
	}
	err = apply_redirections(state, builtin->redirections);
	if (err != NO_ERROR)
	{
		report_t_error("apply_redirections", err);
		return (io_close(io_from_array(io_backup)), command_ok(EXIT_FAILURE));
	}
	res = execute_builtin(state, builtin);
	err = restore_stdin_stdout(io_backup);
	io_close(io_from_array(io_backup));
	if (err != NO_ERROR && res.error == NO_ERROR)
	{
		report_syscall_error(error_repr(err));
		return (command_ok(EXIT_FAILURE));
	}
	return (res);
}

static t_error	save_stdin_stdout(int save[2])
{
	int	in;
	int	out;

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

static t_error	restore_stdin_stdout(int save[2])
{
	int	in;
	int	out;

	in = save[0];
	out = save[1];
	if (dup2(in, STDIN_FILENO) < 0)
		return (E_DUP2);
	if (dup2(out, STDOUT_FILENO) < 0)
		return (E_DUP2);
	return (NO_ERROR);
}
