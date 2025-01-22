/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pidl_last.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:43:59 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:44:05 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_pid_list.h"

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
