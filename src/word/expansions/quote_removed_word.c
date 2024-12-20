#include "./expand.h"
#include "./expand_internals.h"

#include "error/t_error.h"

t_error quote_removed_word(const char *word, char **out)
{
	t_error err;
	t_word_quotes_list *parts;

	err = wql_parse(word, &parts);
	if (err != NO_ERROR)
		return (err);
	wql_remove_outer_quotes(parts);
	err = wql_make_joined(parts, out);
	wql_clear(&parts);
	return (err);
}
