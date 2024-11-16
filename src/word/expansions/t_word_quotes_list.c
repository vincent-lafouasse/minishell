#include "t_word_quotes_list.h"

#include <stdlib.h>

t_error wql_new(t_word_quote_state state, char *part, t_word_quotes_list **out)
{
	*out = malloc(sizeof(t_word_quotes_list));
	if (*out == NULL)
		return (E_OOM);

	**out = (t_word_quotes_list){.next = NULL, .state = state, .part = part};
	return (NO_ERROR);
}

t_word_quotes_list *wql_last(t_word_quotes_list* wql)
{
	t_word_quotes_list* last;

	if (wql == NULL)
		return NULL;
	last = wql;
	while (last->next)
		last = last->next;
	return (last);
}

t_error wql_push_back(t_word_quotes_list** wql, t_word_quote_state state, char *part)
{
	t_error err;
	t_word_quotes_list* new_last;

	err = wql_new(state, part, &new_last);
	if (err != NO_ERROR)
		return err;
	wql_push_back_link(wql, new_last);
	return NO_ERROR;
}

void wql_push_back_link(t_word_quotes_list** wql, t_word_quotes_list* link)
{
	t_word_quotes_list* last;

	if (wql == NULL)
		return ;
	last = wql_last(*wql);
	if (last == NULL)
		*wql = link;
	else
		last->next = link;
}

void wql_delone(t_word_quotes_list **wql)
{
	t_word_quotes_list *next_backup;

	if (!wql || !*wql)
		return;
	next_backup = (*wql)->next;
	free((*wql)->part);
	free(*wql);
	*wql = next_backup;
	return;
}

void wql_clear(t_word_quotes_list** wql)
{
	if (!wql)
		return;
	while (*wql)
		wql_delone(wql);
}
