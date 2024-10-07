#include "t_word_list.h"

#include <stdlib.h>

t_word_list *wl_new(char *contents)
{
	t_word_list* out;

	out = malloc(sizeof(*out));
	if (out == NULL)
		return NULL;
	*out = (t_word_list){.contents = contents, .next = NULL};
	return out;
}

void wl_push_back_link(t_word_list **words, t_word_list *link)
{
	if (words == NULL)
		return;

	if (*words == NULL)
	{
		*words = link;
		return;
	}

	t_word_list* last = *words;

	while (last->next)
		last = last->next;

	last->next = link;
}

t_error wl_push_back(t_word_list **words, char *contents)
{
	t_word_list* new_last = wl_new(contents);

	if (new_last == NULL)
		return E_OOM;
	wl_push_back_link(words, new_last);
	return NO_ERROR;
}
