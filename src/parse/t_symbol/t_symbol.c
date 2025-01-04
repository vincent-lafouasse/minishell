/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_symbol.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:55:41 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:55:42 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_symbol.h"
#include <stdlib.h>

void	*symbol_clear(t_symbol symbol)
{
	size_t	i;

	if (symbol.kind == SYM_TERMINAL)
	{
		free(symbol.token.literal);
		return (NULL);
	}
	if (symbol.kind != SYM_TERMINAL && symbol.production == NULL)
		return (NULL);
	i = 0;
	while (i < symbol.production->len)
	{
		symbol_clear(symbol.production->data[i]);
		i++;
	}
	free(symbol.production);
	symbol.production = NULL;
	return (NULL);
}

t_symbol	symbol_new_non_terminal(t_symbol_kind kind, size_t capacity)
{
	t_symbol_array	*arr;

	arr = symbol_array_with_cap(capacity);
	return ((t_symbol){kind, {.production = arr}});
}

t_symbol	symbol_new_terminal(t_token token)
{
	return ((t_symbol){SYM_TERMINAL, {.token = token}});
}

t_symbol_array	*symbol_array_with_cap(size_t cap)
{
	t_symbol_array	*new;

	new = malloc(sizeof(*new) + sizeof(*new->data) * cap);
	if (!new)
		return (NULL);
	new->len = 0;
	new->cap = cap;
	return (new);
}

bool	symbol_array_push(t_symbol_array *self, t_symbol symbol)
{
	if (self->len == self->cap)
		return (false);
	self->data[self->len++] = symbol;
	return (true);
}
