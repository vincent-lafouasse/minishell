#include "reduction.h"
#include "log/log.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "t_symbol_stack.h"

t_symbol* find_simple_command(t_symbol* root)
{
	t_symbol_stack* waiting_room = ss_new(root);

	while (waiting_room != NULL)
	{
		t_symbol* current = ss_pop(&waiting_room);

		if (current->kind == SIMPLE_COMMAND)
		{
			return current;
		}

		for (int i = current->production->len - 1; i >= 0; i--)
		{
			ss_push(&waiting_room, &current->production->data[i]);
		}
	}

    return NULL;
}

void gather_leaves(t_symbol* root, t_token_list** leaves_p, t_symbol_stack** visited)
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
					gather_leaves(candidate, leaves_p, visited);
		}
    }
}

t_command	reduce_simple_command(t_symbol *root)
{
	t_token_list* leaves = NULL;
    t_symbol_stack* visited = NULL;

    gather_leaves(root, &leaves, &visited);
	log_token_list(leaves);

	return (t_command){};
}
