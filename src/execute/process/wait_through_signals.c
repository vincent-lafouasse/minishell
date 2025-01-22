/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_through_signals.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:49:08 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:50:50 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
* an event other than a process exiting occured, we don't care for it
if (!WIFEXITED(status) && !WIFSIGNALED(status))
	continue ;

// we have caught the termination of a different process than the
// one we were looking for; bash would save this to its global job
// list to keep track of it but since we can't make global
// variables, we have to be careful not to leave any processes
// unaccounted for
if (pid != -1 && waited_for_pid != pid)
	continue ;
*/

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
