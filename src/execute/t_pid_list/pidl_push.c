/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pidl_push.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:44:27 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:46:07 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "t_pid_list.h"

#include <stdlib.h>

void pidl_push_back_link(t_pid_list** pidl, t_pid_list* link)
{
	if (pidl == NULL)
		return ;

	t_pid_list* last = pidl_last(*pidl);

	if (last == NULL)
		*pidl = link;
	else
		last->next = link;
}

void pidl_push_front_link(t_pid_list** pidl, t_pid_list* link)
{
	if (pidl == NULL)
		return ;

	if (*pidl == NULL)
	{
		*pidl = link;
		return ;
	}

	t_pid_list* head = *pidl;
	link->next = head;
	*pidl = link;
}


t_error pidl_push_back(t_pid_list** pidl, pid_t pid)
{
	t_pid_list* new_last = pidl_new(pid);

	if (new_last == NULL)
		return E_OOM;
	pidl_push_back_link(pidl, new_last);
	return NO_ERROR;
}

t_error pidl_push_front(t_pid_list** pidl, pid_t pid)
{
	t_pid_list* new_head = pidl_new(pid);

	if (new_head == NULL)
		return E_OOM;
	pidl_push_front_link(pidl, new_head);
	return NO_ERROR;
}
