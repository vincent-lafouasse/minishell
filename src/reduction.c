#include "reduction.h"
#include "log/log.h"
#include "parse/t_command/t_command.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "t_symbol_stack.h"

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

#include <stdlib.h>

static t_redir_kind redir_kind_from_angle_bracket(t_token_type bracket)
{
	if (bracket == L_ANGLE_BRACKET)
		return (FROM_FILE);
	else if (bracket == R_ANGLE_BRACKET)
		return (INTO_FILE);
	else if (bracket == DL_ANGLE_BRACKET)
		return (HERE_DOCUMENT);
	else if (bracket == DR_ANGLE_BRACKET)
		return (APPEND_INTO_FILE);
	return (0);
}

t_command	reduce_simple_command(t_symbol *root)
{
	t_token_list	*leaves;
	t_simple		*cmd;

	assert (root->kind == SIMPLE_COMMAND);
	leaves = gather_leaves(root);
	assert (leaves != NULL);
	cmd = malloc(sizeof(*cmd));
	assert (cmd != NULL);
	*cmd = (t_simple){0};
	while (leaves)
	{
		if (leaves->token.type == WORD)
		{
			wl_push_back(&cmd->words, leaves->token.literal);
			leaves = leaves->next;
		}
		else
		{
			t_redirect	redir;
			t_token		bracket;
			t_token		word;

			bracket = leaves->token;
			word = leaves->next->token;
			redir.kind = redir_kind_from_angle_bracket(bracket.type);
			if (redir.kind == HERE_DOCUMENT)
				redir.doc = (t_here_doc){NULL, word.literal};
			else
				redir.filename = word.literal;
			rdl_push_back(&cmd->redirections, redir);
			leaves = leaves->next->next;
		}
	}
	return (t_command){.type = SIMPLE_CMD, .simple = cmd};
}
