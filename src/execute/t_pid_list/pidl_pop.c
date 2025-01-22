/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pidl_pop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:46:27 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:46:35 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_pid_list.h"

#include <stdlib.h>

t_pid_list* pidl_pop_front_link(t_pid_list** pidl)
{
	if (pidl == NULL || *pidl == NULL)
		return NULL;

	t_pid_list* out = *pidl;
	t_pid_list* new_first = out->next;
	out->next = NULL;
	*pidl = new_first;
	return out;
}

pid_t pidl_pop_front(t_pid_list** pidl)
{
	t_pid_list* front = pidl_pop_front_link(pidl);

	if (front == NULL)
		return 0;

	pid_t out = front->pid;
	free(front);
	return out;
}
