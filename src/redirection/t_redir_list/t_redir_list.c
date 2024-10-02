#include "t_redir_list.h"

#include <stdlib.h>

t_redir_list *rdl_new(char *contents)
{
	t_redir_list* out;

	out = malloc(sizeof(*out));
	if (!out)
		return NULL;
	*out = (t_redir_list){.redirect = {.filename = contents}, .next = NULL};
	return out;
}

void rdl_push_back_link(t_redir_list **words, t_redir_list *link)
{
	if (!words)
		return;

	if (*words == NULL)
	{
		*words = link;
		return;
	}

	t_redir_list* last = *words;
	while (last->next)
		last = last->next;

	last->next = link;
}
