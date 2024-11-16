#include "./expand.h"
#include "./expand_internals.h"

#include "word/t_word_list/t_word_list.h"
#include "error/t_error.h"

#include <stdlib.h>
#include <stdbool.h>

t_error perform_all_expansions_on_words(t_word_list *word)
{
	(void)word;
	return (NO_ERROR); // bad, dummy `perform_all_expansions_on_words` implementation
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
