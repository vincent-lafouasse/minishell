/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:36:30 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:37:18 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/t_error.h"
#include "libft/ft_io.h"
#include "process.h"
#include "shell/shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

t_error	wait_for_process(t_state *state, pid_t pid, int *exit_status_out)
{
	int		status;
	pid_t	waited_for_pid;

	waited_for_pid = wait_through_signals(pid, &status);
	if (waited_for_pid < 0)
		return (E_WAIT);
	if (WIFSIGNALED(status))
	{
		if (state->is_interactive && state->tty_properties_initialized)
			reset_tty_properties(STDERR_FILENO, &state->tty_properties);
		if (WTERMSIG(status) != SIGINT && WTERMSIG(status) != SIGPIPE)
			report_signal_related_exit(status);
		else if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
	}
	*exit_status_out = get_exit_status(status);
	return (NO_ERROR);
}
