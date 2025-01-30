/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:35:26 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:35:27 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/t_error.h"
#include "process.h"
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

t_error	fork_process(bool *in_child, pid_t *pid_out)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (E_FORK);
	if (in_child != NULL)
		*in_child = pid == 0;
	if (pid_out != NULL && pid != 0)
		*pid_out = pid;
	return (NO_ERROR);
}

// it is cheaper to clean up one allocation than it is to clean up a fork in
// the event of an error; optimistically allocate a link here
t_error	fork_and_push_pid(bool *in_child, t_pid_list **pidl_out)
{
	t_error		err;
	t_pid_list	*link;

	link = pidl_new(0);
	if (!link)
		return (E_OOM);
	err = fork_process(in_child, &link->pid);
	if (err != NO_ERROR)
	{
		pidl_clear(&link);
		return (err);
	}
	if (*in_child)
		pidl_clear(&link);
	else
		pidl_push_back_link(pidl_out, link);
	return (NO_ERROR);
}
