#include "libft/ft_string.h"
#include "libft/string.h"
#include "t_word_quotes_list.h"

#include <assert.h> // temporarily
#include <stdlib.h>

static t_word_quote_state current_word_part_kind(const char *start)
{
	if (*start == '\'')
		return (WQS_SINGLY_QUOTED);
	else if (*start == '"')
		return (WQS_DOUBLY_QUOTED);
	else
		return (WQS_UNQUOTED);
}

static const char *find_next_quote_or_nul_char(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '"' && str[i] != '\'')
		i++;
	return (&str[i]);
}

static size_t	current_word_part_len(const char *start)
{
	const char *next_quote_or_nul;
	size_t len;

	len = 0;
	if (*start == '\0')
		return 0;
	next_quote_or_nul = find_next_quote_or_nul_char(start + 1);
	if (*start == '"' || *start == '\'')
	{
		if (*next_quote_or_nul != *start) /* quotes don't match for some reason? */
			return (next_quote_or_nul - start);
		len = next_quote_or_nul + 1 - start;
	}
	else
		len = next_quote_or_nul - start;
	return (len);
}

t_error wql_parse(const char *word, t_word_quotes_list **out)
{
	t_word_quotes_list *quotes;
	size_t current_part_len;
	t_word_quote_state current_part_kind;
	t_error err;
	char *part;

	quotes = NULL;
	while (*word)
	{
		current_part_kind = current_word_part_kind(word);
		current_part_len = current_word_part_len(word);
		part = ft_substr(word, 0, current_part_len);
		if (!part)
		{
			wql_clear(&quotes);
			return (E_OOM);
		}
		err = wql_push_back(&quotes, current_part_kind, part);
		if (err != NO_ERROR)
		{
			free(part);
			wql_clear(&quotes);
			return (E_OOM);
		}
		word += current_part_len;
	}
	*out = quotes;
	return (NO_ERROR);
}
