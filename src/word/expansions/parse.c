/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:45:47 by poss              #+#    #+#             */
/*   Updated: 2025/01/31 14:51:33 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internals.h"
#include "libft/ft_string.h"
#include "libft/string.h"
#include <assert.h> // temporarily
#include <stdlib.h>

static t_word_quote_state	current_word_part_kind(const char *start)
{
	if (*start == '\'')
		return (WQS_SINGLY_QUOTED);
	else if (*start == '"')
		return (WQS_DOUBLY_QUOTED);
	else
		return (WQS_UNQUOTED);
}

static const char	*find_next_quote_or_nul_char(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '"' && str[i] != '\'')
		i++;
	return (&str[i]);
}

static size_t	current_word_part_len(const char *start)
{
	const char	*next_quote;
	size_t		len;

	len = 0;
	if (*start == '\0')
		return (0);
	if (*start == '"' || *start == '\'')
	{
		next_quote = ft_strchr(start + 1, *start);
		if (!next_quote)
			return (next_quote - start);
		len = next_quote + 1 - start;
	}
	else
		len = find_next_quote_or_nul_char(start + 1) - start;
	return (len);
}

t_error	wql_parse(const char *word, t_word_quotes_list **out)
{
	t_word_quotes_list	*quotes;
	size_t				current_part_len;
	t_word_quote_state	current_part_kind;
	t_error				err;
	char				*part;

	quotes = NULL;
	while (*word)
	{
		current_part_kind = current_word_part_kind(word);
		current_part_len = current_word_part_len(word);
		part = ft_substr(word, 0, current_part_len);
		if (!part)
			return (wql_clear(&quotes), E_OOM);
		err = wql_push_back(&quotes, current_part_kind, part);
		if (err != NO_ERROR)
			return (free(part), wql_clear(&quotes), E_OOM);
		word += current_part_len;
	}
	*out = quotes;
	return (NO_ERROR);
}
