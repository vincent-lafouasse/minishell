/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_joined.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:45:45 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:45:46 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internals.h"
#include "libft/string.h"
#include <stdlib.h>

static size_t	wql_cumulated_length(const t_word_quotes_list *wql)
{
	size_t	len;

	len = 0;
	while (wql)
	{
		len += ft_strlen(wql->part);
		wql = wql->next;
	}
	return (len);
}

static bool	any_words_were_quoted(const t_word_quotes_list *wql)
{
	while (wql)
	{
		if (wql->quotes_removed == true)
			return (true);
		wql = wql->next;
	}
	return (false);
}

static bool	expands_to_nonempty_word_list(const t_word_quotes_list *wql)
{
	return (wql_cumulated_length(wql) != 0 || any_words_were_quoted(wql));
}

t_error	wql_make_joined(const t_word_quotes_list *wql, char **out)
{
	char	*joined;
	size_t	current_part_len;
	size_t	i;

	if (!expands_to_nonempty_word_list(wql))
	{
		*out = NULL;
		return (NO_ERROR);
	}
	joined = malloc((wql_cumulated_length(wql) + 1) * sizeof(char));
	if (!joined)
		return (E_OOM);
	i = 0;
	while (wql)
	{
		current_part_len = ft_strlen(wql->part);
		ft_memcpy(joined + i, wql->part, current_part_len);
		i += current_part_len;
		wql = wql->next;
	}
	joined[i] = '\0';
	*out = joined;
	return (NO_ERROR);
}
