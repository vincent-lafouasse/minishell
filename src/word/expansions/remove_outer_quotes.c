/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_outer_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:45:57 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:45:57 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internals.h"

#include "libft/string.h"

#include <stdbool.h>

static bool is_malformed_quoted_string(const char *str)
{
	size_t len;

	len = ft_strlen(str);
	if (len == 0 || len == 1)
		return (true);
	if (str[len - 1] != *str)
		return (true);
	return (false);
}

// with extra checks to ensure no UB occurs
static void remove_outer_quotes_inner(t_word_quotes_list *current)
{
	size_t len;

	if (is_malformed_quoted_string(current->part))
		return;
	len = ft_strlen(current->part);
	ft_memmove(current->part, current->part + 1, len - 2);
	current->part[len - 2] = '\0';
	current->quotes_removed = true;
}

void	wql_remove_outer_quotes(t_word_quotes_list *wql)
{
	while (wql)
	{
		if (wql->state != WQS_UNQUOTED)
			remove_outer_quotes_inner(wql);
		wql = wql->next;
	}
}
