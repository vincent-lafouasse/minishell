/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:00:31 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 18:02:01 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/t_error.h"
#include "execute.h"
#include "execute/process/process.h"
#include "libft/string.h"
#include "parse/t_command/t_command.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define IGNORED_REDIR_SUBSHELL \
		"minishell: warning: ignored redirs in subshell\n"

static void	warn_non_empty_redirs(const t_subshell *s)
{
	if (s->redirections != NULL)
		write(STDERR_FILENO, IGNORED_REDIR_SUBSHELL,
			ft_strlen(IGNORED_REDIR_SUBSHELL));
}

t_launch_result	launch_subshell(t_state *state, t_subshell *subshell, t_io io,
		int fd_to_close)
{
	t_error				err;
	t_pid_list			*pids;
	pid_t				pid;
	t_command_result	inner_res;

	pids = pidl_new(0);
	if (pids == NULL)
	{
		free(pids);
		return ((t_launch_result){.error = E_OOM, .pids = NULL});
	}
	pid = fork();
	if (pid == -1)
	{
		free(pids);
		return ((t_launch_result){.error = E_FORK, .pids = NULL});
	}
	if (pid != 0)
	{
		pids->pid = pid;
		return ((t_launch_result){.error = NO_ERROR, .pids = pids});
	}
	err = do_piping(io);
	if (err != NO_ERROR)
		perror("dup2");
	warn_non_empty_redirs(subshell);
	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);
	inner_res = execute_command(state, subshell->cmd); // bad?, log err ?
	exit(inner_res.status_code); // bad. dont know what status to return yet
}

t_command_result	execute_subshell(t_state *state, t_subshell *subshell)
{
	t_launch_result	launch_result;
	pid_t			pid;
	int				exit_status;
	t_error			err;

	launch_result = launch_subshell(state, subshell, io_default(),
			CLOSE_NOTHING);
	if (launch_result.error != NO_ERROR)
	{
		return ((t_command_result){.error = launch_result.error});
	}
	pid = launch_result.pids->pid;
	err = wait_for_process(state, pid, &exit_status);
	if (err != NO_ERROR)
		return (/* kill(pid, SIGKILL), */ (t_command_result){.error = err});
	return ((t_command_result){.error = NO_ERROR, .status_code = exit_status});
}
