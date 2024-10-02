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

static bool recurse_pipeline(t_command *out, t_symbol *pipeline_rest)
{
	t_symbol_array	*productions;
	t_pipeline		*pipeline;
	t_command		first;
	t_command		second;

	productions = pipeline_rest->production;

	if (productions->len == 0)
		return (false);

	first = reduce_command(&productions->data[1]);
	if (!recurse_pipeline(&second, &productions->data[2]))
	{
		*out = first;
		return (true);
	}

	pipeline = malloc(sizeof(*pipeline));
	assert (pipeline != NULL);

	*pipeline = (t_pipeline){.first = first, .second = second};
	*out = (t_command){.type = PIPELINE_CMD, .pipeline = pipeline};

	return true;
}

t_command	reduce_pipeline_rest(t_symbol *pipeline_rest)
{
	t_command out;

	assert (pipeline_rest->kind == PIPELINE_REST);
	assert (pipeline_rest->production->len > 0);

	recurse_pipeline(&out, pipeline_rest);

	return out;
}

t_command	reduce_pipeline(t_symbol *pipeline)
{
	t_pipeline	*out;

	assert (pipeline->kind == PIPELINE);

	if (pipeline->production->data[1].production->len == 0)
		return reduce_command(&pipeline->production->data[0]);

	out = malloc(sizeof(*out));
	assert (out != NULL);

	out->first = reduce_command(&pipeline->production->data[0]);
	out->second = reduce_pipeline_rest(&pipeline->production->data[1]);

	return (t_command){.type = PIPELINE_CMD, .pipeline = out};
}

t_command	reduce_subshell(t_symbol *subshell, t_symbol *trailing_redirs);

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

t_command	reduce_complete_command(t_symbol *root);

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
