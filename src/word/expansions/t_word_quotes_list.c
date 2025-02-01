/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_word_quotes_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:45:59 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:48:59 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internals.h"
#include <stdlib.h>

t_error	wql_new(t_word_quote_state state, char *part, t_word_quotes_list **out)
{
	*out = malloc(sizeof(t_word_quotes_list));
	if (*out == NULL)
		return (E_OOM);
	**out = (t_word_quotes_list){.next = NULL, .state = state, .part = part};
	return (NO_ERROR);
}

t_word_quotes_list	*wql_last(t_word_quotes_list *wql)
{
	t_word_quotes_list	*last;

	if (wql == NULL)
		return (NULL);
	last = wql;
	while (last->next)
		last = last->next;
	return (last);
}

void	wql_delone(t_word_quotes_list **wql)
{
	t_word_quotes_list	*next_backup;

	if (!wql || !*wql)
		return ;
	next_backup = (*wql)->next;
	free((*wql)->part);
	free(*wql);
	*wql = next_backup;
	return ;
}

void	wql_clear(t_word_quotes_list **wql)
{
	if (!wql)
		return ;
	while (*wql)
		wql_delone(wql);
}
