#include "parse/t_command/t_command.h"
#include "parse/t_symbol.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "t_symbol_stack.h"

t_token_list* gather_leaves(t_symbol* root)
{
    t_token_list* leaves = NULL;
	t_symbol_stack* waiting_room = ss_new(root);

	while (waiting_room != NULL)
	{
		t_symbol* current = ss_pop(&waiting_room);

		if (current->kind == TERMINAL)
		{
			tkl_push_back(&leaves, current->token);
			// free the terminal ?
			continue;
		}

		for (int i = current->production->len - 1; i >= 0; i--)
		{
			ss_push(&waiting_room, &current->production->data[i]);
		}
		// free current ?
	}

    return leaves;
}

t_command	reduce_simple_command(t_symbol *root)
{
	t_token_list* leaves = gather_leaves(root);

	return (t_command){};
}
