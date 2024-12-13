#include "./expand.h"
#include "./expand_internals.h"

#include "error/t_error.h"

t_error quote_removed_word(const char *word, char **out)
{
	t_error err;
	t_word_quotes_list *parts;
	char *res;

	err = wql_parse(word, &parts);
	if (err != NO_ERROR)
		return (err);
	wql_remove_outer_quotes(parts);
	err = wql_make_joined(parts, &res);
	wql_clear(&parts);
	*out = res;
	return (err);
}
