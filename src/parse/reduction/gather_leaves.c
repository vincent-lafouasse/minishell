#include "reduction_internals.h"
#include "parse/t_symbol/t_symbol_stack/t_symbol_stack.h"

static void recurse(t_symbol* root, t_token_list** leaves_p, t_symbol_stack** visited)
{
    ss_push(visited, root);
    if (root->kind == TERMINAL)
    {
        tkl_push_back(leaves_p, root->token);
    }
    else
    {
		for (size_t i = 0; i < root->production->len; i++)
		{
		    t_symbol* candidate = &root->production->data[i];
		    if (!ss_contains(*visited, candidate))
				recurse(candidate, leaves_p, visited);
		}
    }
}

t_token_list *gather_leaves(t_symbol* root)
{
	t_token_list* leaves;
    t_symbol_stack* visited;

	leaves = NULL;
	visited = NULL;
    recurse(root, &leaves, &visited);
	//ss_clear(&visited);
	return (leaves);
}
