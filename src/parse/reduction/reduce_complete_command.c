#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include "tokenize/t_token.h"
#include <stdbool.h>

#include <stdlib.h> // temporarily
#include <assert.h> // temporarily

static t_conditional_operator operator_from_token_type(t_token_type type)
{
	if (type == AND_AND)
		return AND_OP;
	else if (type == OR_OR)
		return OR_OP;
	assert(true);
	return (0);
}

static bool recurse(t_command *out, t_symbol *complete_cmd_rest)
{
	t_symbol_array			*productions;
	t_conditional			*conditional;
	t_conditional_operator	operator;
	t_command				first;
	t_command				second;

	assert(complete_cmd_rest->kind == COMPLETE_COMMAND_REST);

	productions = complete_cmd_rest->production;

	if (productions->len == 0)
		return (false);

	first = reduce_pipeline(&productions->data[1]);
	if (!recurse(&second, &productions->data[2]))
	{
		*out = first;
		return (true);
	}

	conditional = malloc(sizeof(*conditional));
	assert (conditional != NULL);

	// get the operator from the next `complete_command_rest`, as we know it
	// exists
	operator = operator_from_token_type(productions->data[2].production->data[0].token.type);
	*conditional = (t_conditional){
		.operator = operator,
		.first = first,
		.second = second
	};
	*out = (t_command){.type = CONDITIONAL_CMD, .conditional = conditional};

	return true;
}

static t_command	reduce_complete_cmd_rest(t_symbol *symbol)
{
	t_command out;

	assert (symbol->kind == COMPLETE_COMMAND_REST);
	assert (symbol->production->len > 0);

	recurse(&out, symbol);

	return out;
}

t_command	reduce_complete_command(t_symbol *root)
{
	t_conditional	*out;

	assert (root->kind == COMPLETE_COMMAND);

	if (root->production->data[1].production->len == 0)
		return reduce_pipeline(&root->production->data[0]);

	out = malloc(sizeof(*out));
	assert (out != NULL);

	out->operator = operator_from_token_type(root->production->data[1].production->data[0].token.type);
	out->first = reduce_pipeline(&root->production->data[0]);
	out->second = reduce_complete_cmd_rest(&root->production->data[1]);

	return (t_command){.type = CONDITIONAL_CMD, .conditional = out};
}