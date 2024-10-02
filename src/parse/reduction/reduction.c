#include "reduction.h"
#include "reduction_internals.h"
#include "log/log.h"
#include "parse/t_command/t_command.h"
#include "parse/t_symbol/t_symbol.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "parse/t_symbol/t_symbol_stack/t_symbol_stack.h"

#include <assert.h>

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

#include <stdlib.h>

t_command	reduce_complete_command(t_symbol *root)
{
	(void)root;
	return (t_command){0};
}

// command : simple_command
//         | subshell redirect_list
//         | subshell
t_command	reduce_command(t_symbol *root)
{
	assert (root->kind == COMMAND);
	if (root->production->data[0].kind == SIMPLE_COMMAND)
		return reduce_simple_command(&root->production->data[0]);
	else
		return reduce_subshell(&root->production->data[0], \
							   &root->production->data[1]);
}

t_command	reduce_subshell(t_symbol *subshell, t_symbol *trailing_redirs)
{
	t_subshell	*sub;

	assert (subshell->kind == SUBSHELL);
	assert (trailing_redirs->kind == SUBSHELL_PRECEDES);

	sub = malloc(sizeof(*sub));
	assert (sub != NULL);

	sub->cmd = reduce_complete_command(subshell);
	reduce_simple_command_like(trailing_redirs, NULL, &sub->redirections);

	return (t_command){.type = SUBSHELL_CMD, .subshell = sub};
}

t_command	reduce_simple_command(t_symbol *root)
{
	t_simple		*simple;

	assert (root->kind == SIMPLE_COMMAND);
	simple = malloc(sizeof(*simple));
	assert (simple != NULL);
	*simple = (t_simple){0};
	reduce_simple_command_like(root, &simple->words, &simple->redirections);
	return (t_command){.type = SIMPLE_CMD, .simple = simple};
}
