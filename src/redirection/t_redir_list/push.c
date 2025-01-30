/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:51:39 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:51:52 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_redir_list.h"

void	rdl_push_back_link(t_redir_list **words, t_redir_list *link)
{
	t_redir_list	*last;

	if (!words)
		return ;
	if (*words == NULL)
	{
		*words = link;
		return ;
	}
	last = *words;
	while (last->next)
		last = last->next;
	last->next = link;
}

t_error	rdl_push_back(t_redir_list **words, t_redirect redir)
{
	t_redir_list	*new_last;

	new_last = rdl_new(redir);
	if (new_last == NULL)
		return (E_OOM);
	rdl_push_back_link(words, new_last);
	return (NO_ERROR);
}
