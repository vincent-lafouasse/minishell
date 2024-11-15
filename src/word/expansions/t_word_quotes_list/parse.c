#include "libft/ft_string.h"
#include "libft/string.h"
#include "t_word_quotes_list.h"

#include <assert.h>
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

static size_t	current_word_part_len(const char *start)
{
	const char *next_quote;
	size_t len;

	len = 0;
	if (*start == '"' || *start == '\'')
	{
		next_quote = ft_strchr(start + 1, *start);
		assert(next_quote != NULL);
		len = next_quote - start;
	}
	else
	{
		while (start[len] != '"' && start[len] != '\'')
			len++;
	}
	return (len);
}

t_error wql_parse(const char *compound_word, t_word_quotes_list **out)
{
	t_word_quotes_list *quotes;
	size_t current_part_len;
	t_word_quote_state current_part_kind;
	t_error err;
	char *part;

	quotes = NULL;
	while (*compound_word)
	{
		current_part_kind = current_word_part_kind(compound_word);
		current_part_len = current_word_part_len(compound_word);
		part = ft_substr(compound_word, 0, current_part_len);
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
		compound_word += current_part_len;
	}
	*out = quotes;
	return (NO_ERROR);
}
