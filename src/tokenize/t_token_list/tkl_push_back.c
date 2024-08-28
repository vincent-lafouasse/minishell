#include "t_token_list.h"

void tkl_push_back_link(t_token_list **tokens_p, t_token_list *link)
{
	t_token_list* current;

	if (!tokens_p || !link)
		return;
	if (!*tokens_p)
	{
		*tokens_p = link;
		return;
	}
	current = *tokens_p;
	while (current->next)
		current = current->next;
	current->next = link;
}

t_error	tkl_push_back(t_token_list **tokens_p, t_token token)
{
	t_token_list* new_link;

	new_link = tkl_new(token);
	if (!new_link)
		return E_OOM;
	tkl_push_back_link(tokens_p, new_link);
	return (NO_ERROR);
}
