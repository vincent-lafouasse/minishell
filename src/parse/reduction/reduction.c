#include "reduction.h"
#include "reduction_internals.h"
#include "log/log.h"
#include "parse/t_command/t_command.h"
#include "parse/t_symbol/t_symbol.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "parse/t_symbol/t_symbol_stack/t_symbol_stack.h"

t_symbol* find_symbol(t_symbol* root, t_symbol_kind kind)
{
	t_symbol_stack* waiting_room = ss_new(root);

	while (waiting_room != NULL)
	{
		t_symbol* current = ss_pop(&waiting_room);

		if (current->kind == kind)
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

t_command	reduce_parse_tree_into_command(t_symbol *parse_tree)
{
	t_command out;

	out = reduce_complete_command(parse_tree);
	symbol_clear(*parse_tree);
	return out;
}
