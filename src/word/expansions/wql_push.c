/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wql_push.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:48:37 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:48:50 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internals.h"

t_error	wql_push_back(t_word_quotes_list **wql, t_word_quote_state state,
		char *part)
{
	t_error				err;
	t_word_quotes_list	*new_last;

	err = wql_new(state, part, &new_last);
	if (err != NO_ERROR)
		return (err);
	wql_push_back_link(wql, new_last);
	return (NO_ERROR);
}

void	wql_push_back_link(t_word_quotes_list **wql, t_word_quotes_list *link)
{
	t_word_quotes_list	*last;

	if (wql == NULL)
		return ;
	last = wql_last(*wql);
	if (last == NULL)
		*wql = link;
	else
		last->next = link;
}
