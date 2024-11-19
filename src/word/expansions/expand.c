#include "./expand.h"
#include "./expand_internals.h"

#include "word/t_word_list/t_word_list.h"
#include "error/t_error.h"

#include <stdlib.h>
#include <stdbool.h>

t_error variable_expand_words(t_expansion_variables vars, t_word_list **words) // TODO: do field splitting
{
	char *expanded_word;
	t_error err;
	t_word_list *current;

	current = *words;
	while (current)
	{
		err = variable_expand_word(vars, current->contents, &expanded_word);
		if (err != NO_ERROR)
			return (err);
		if (!expanded_word && current->prev == NULL)
		{
			wl_delone(words, free);
			current = *words;
		}
		else if (!expanded_word && current->prev != NULL) {
			wl_delone(&current, free);
		}
		else
		{
			free(current->contents);
			current->contents = expanded_word;
			current = current->next;
		}
	}
	return (NO_ERROR);
}

t_error variable_expand_word(t_expansion_variables vars, const char *word, char **out)
{
	t_error err;
	t_word_quotes_list *parts;

	err = wql_parse(word, &parts);
	if (err != NO_ERROR)
		return (err);
	err = wql_variable_expand(vars, parts);
	if (err != NO_ERROR)
	{
		wql_clear(&parts);
		return (err);
	}
	wql_remove_outer_quotes(parts);
	err = wql_make_joined(parts, out);
	if (err != NO_ERROR)
	{
		wql_clear(&parts);
		return (err);
	}
	wql_clear(&parts);
	return (NO_ERROR);
}
