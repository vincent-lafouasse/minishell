#include "parse/tokenize/t_token.h"
#include "reduction_internals.h"
#include "../tokenize/t_token_list/t_token_list.h"

static t_error recurse(t_symbol* root, t_token_list** leaves_p)
{
	size_t	i;
	t_error	err;
	t_token copy;

	i = 0;
	if (root->kind == TERMINAL)
	{
		err = token_duplicate(root->token, &copy);
		if (err != NO_ERROR)
			return (err);
		return tkl_push_back(leaves_p, copy);
	}
	while (i < root->production->len)
	{
		err = recurse(&root->production->data[i], leaves_p);
		if (err != NO_ERROR)
			return err;
		i++;
	}
	return (NO_ERROR);
}

t_error gather_leaves(t_symbol* root, t_token_list **out)
{
    t_error err;

    err = recurse(root, out);
	if (err != NO_ERROR)
	{
		tkl_clear(out, NULL);
		return err;
	}
	return (NO_ERROR);
}
