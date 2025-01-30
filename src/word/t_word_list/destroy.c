/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:36:41 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:36:57 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_word_list.h"
#include <stdlib.h>

void	wl_delone(t_word_list **words, t_destructor del)
{
	t_word_list	*next;
	t_word_list	*prev;

	if (!words || !*words)
		return ;
	next = (*words)->next;
	prev = (*words)->prev;
	if (del)
		del((*words)->contents);
	free(*words);
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;
	*words = next;
	return ;
}

void	wl_clear(t_word_list **words, t_destructor del)
{
	if (!words)
		return ;
	while (*words)
		wl_delone(words, del);
}
