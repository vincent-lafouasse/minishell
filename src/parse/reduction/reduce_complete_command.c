#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"
#include "../tokenize/t_token.h"
#include "error/t_error.h"

#include <stdbool.h>

#include <assert.h> // temporarily

static t_conditional_operator operator_from_token_type(t_token_type type)
{
	if (type == AND_AND)
		return AND_OP;
	else if (type == OR_OR)
		return OR_OP;
	assert(false);
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

	// bad, should check for errs
	reduce_pipeline(&productions->data[1], &first);
	if (!recurse(&second, &productions->data[2]))
	{
		*out = first;
		return (true);
	}

	// get the operator from the next `complete_command_rest`, as we know it
	// exists
	operator = operator_from_token_type(productions->data[2].production->data[0].token.type);

	conditional = conditional_new(operator, first, second);
	assert (conditional != NULL);

	*out = command_from_conditional(conditional);

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

t_error	reduce_complete_command(t_symbol *root, t_command *out)
{
	t_error err;

	assert (root->kind == COMPLETE_COMMAND);

	if (root->production->data[1].production->len == 0)
		return reduce_pipeline(&root->production->data[0], out);

	out->conditional = conditional_new(0, (t_command){0}, (t_command){0});
	assert (out != NULL);

	out->conditional->op = operator_from_token_type(root->production->data[1].production->data[0].token.type);
	err = reduce_pipeline(&root->production->data[0], &out->conditional->first);
	if (err != NO_ERROR)
		return (err);
	// bad, should check for error
	out->conditional->second = reduce_complete_cmd_rest(&root->production->data[1]);

	*out = command_from_conditional(out->conditional);
	return (NO_ERROR);
}
