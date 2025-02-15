/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_through_signals.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:36:32 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:40:36 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

// # `!WIFEXITED(status) && !WIFSIGNALED(status)`
// an event other than a process exiting occured, we don't care for it
//
// # `pid != -1 && waited_for_pid != pid`
//
// we have caught the termination of a different process than the
// one we were looking for; bash would save this to its global job
// list to keep track of it but since we can't make global
// variables, we have to be careful not to leave any processes
// unaccounted for
pid_t	wait_through_signals(pid_t pid, int *status_out)
{
	pid_t	waited_for_pid;
	int		status;

	while (true)
	{
		waited_for_pid = waitpid(-1, &status, 0);
		if (waited_for_pid < 0 && errno == EINTR)
			continue ;
		if (waited_for_pid >= 0)
		{
			if (!WIFEXITED(status) && !WIFSIGNALED(status))
				continue ;
			if (pid != -1 && waited_for_pid != pid)
				continue ;
		}
		break ;
	}
	if (waited_for_pid >= 0 && status_out)
		*status_out = status;
	return (waited_for_pid);
}
