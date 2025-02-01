/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_pid_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:32:18 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:34:10 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_pid_list.h"
#include <stdlib.h>

t_pid_list	*pidl_new(pid_t pid)
{
	t_pid_list	*out;

	out = malloc(sizeof(*out));
	if (out == NULL)
		return (NULL);
	*out = (t_pid_list){.pid = pid, .next = NULL};
	return (out);
}

t_pid_list	*pidl_last(const t_pid_list *pidl)
{
	t_pid_list	*last;

	if (pidl == NULL)
		return (NULL);
	last = (t_pid_list *)pidl;
	while (last->next)
		last = last->next;
	return (last);
}

void	pidl_delone(t_pid_list **pidl)
{
	t_pid_list	*buffer;

	if (!pidl || !*pidl)
		return ;
	buffer = (*pidl)->next;
	free(*pidl);
	*pidl = buffer;
	return ;
}

void	pidl_clear(t_pid_list **pidl)
{
	if (!pidl)
		return ;
	while (*pidl)
		pidl_delone(pidl);
}
