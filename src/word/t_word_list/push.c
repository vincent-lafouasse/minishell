/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:37:16 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:37:28 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_word_list.h"
#include <stdlib.h>

void	wl_push_back_link(t_word_list **words, t_word_list *link)
{
	t_word_list	*last;

	if (words == NULL)
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
	link->prev = last;
}

t_error	wl_push_back(t_word_list **words, char *contents)
{
	t_word_list	*new_last;

	new_last = wl_new(contents);
	if (new_last == NULL)
		return (E_OOM);
	wl_push_back_link(words, new_last);
	return (NO_ERROR);
}
