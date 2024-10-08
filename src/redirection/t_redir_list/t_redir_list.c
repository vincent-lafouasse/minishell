#include "t_redir_list.h"

#include <stdlib.h>

t_redir_list *rdl_new(t_redirect redir)
{
	t_redir_list* out;

	out = malloc(sizeof(*out));
	if (!out)
		return NULL;
	*out = (t_redir_list){.redirect = redir, .next = NULL};
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

t_error rdl_push_back(t_redir_list **words, t_redirect redir)
{
	t_redir_list* new_last = rdl_new(redir);

	if (new_last == NULL)
		return E_OOM;
	rdl_push_back_link(words, new_last);
	return NO_ERROR;
}

void rdl_clear(t_redir_list **rdl, t_destructor del)
{
	(void)del;
	*rdl = NULL;
}
