/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:36:28 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:36:29 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../t_pid_list/t_pid_list.h"
#include "error/t_error.h"
#include "libft/ft_io.h"
#include "process.h"
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

static t_error	waitpid_and_exhaust_children(pid_t pid, int *status_out)
{
	pid_t	waited_for_pid;
	bool	children_remain;
	bool	requested_pid_exited;
	int		status;

	requested_pid_exited = false;
	children_remain = true;
	while (children_remain)
	{
		waited_for_pid = wait_through_signals(-1, &status);
		if (waited_for_pid < 0)
		{
			if (errno == ECHILD)
				children_remain = false;
			else
				return (E_WAIT);
		}
		if (waited_for_pid == pid)
		{
			*status_out = status;
			requested_pid_exited = true;
		}
	}
	if (!requested_pid_exited)
		return (E_WAIT);
	return (NO_ERROR);
}

t_error	wait_for_pipeline(t_state *state, const t_pid_list *pids,
		int *last_exit_status_out)
{
	pid_t	last_pid;
	int		last_status;
	t_error	err;

	if (!pids)
	{
		*last_exit_status_out = 0;
		return (NO_ERROR);
	}
	last_pid = pidl_last(pids)->pid;
	err = waitpid_and_exhaust_children(last_pid, &last_status);
	if (err != NO_ERROR)
		return (err);
	if (WIFSIGNALED(last_status))
	{
		if (state->is_interactive && state->tty_properties_initialized)
			reset_tty_properties(STDERR_FILENO, &state->tty_properties);
		if (WTERMSIG(last_status) != SIGINT && WTERMSIG(last_status) != SIGPIPE)
			report_signal_related_exit(last_status);
		else if (WTERMSIG(last_status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
	}
	*last_exit_status_out = get_exit_status(last_status);
	return (NO_ERROR);
}
