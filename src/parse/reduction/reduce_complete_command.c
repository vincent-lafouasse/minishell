#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"
#include "../tokenize/t_token.h"
#include "error/t_error.h"
#include "libft/stdlib.h"
#include "reduction_internals.h"
#include <assert.h> // temporarily
#include <stdbool.h>
#include <stdio.h> // temporarily
#include <stdlib.h>

static t_conditional_operator	operator_from_token_type(t_token_type type)
{
	if (type == AND_AND)
		return (AND_OP);
	else if (type == OR_OR)
		return (OR_OP);
	assert(false);
	return (0);
}

static t_error	reduce_complete_cmd_rest(t_symbol *cmd_rest, t_command *out)
{
	t_symbol_array			*productions;
	t_symbol				next_cmd_rest;
	t_conditional_operator	operator;
	t_error					err;

	assert(cmd_rest->kind == COMPLETE_COMMAND_REST);
	productions = cmd_rest->production;
	next_cmd_rest = productions->data[2];
	if (next_cmd_rest.production->len == 0)
		return (reduce_pipeline(&productions->data[1], out));
	operator= operator_from_token_type(next_cmd_rest.production->data[0].token.type);
	*out = command_new_conditional(operator,(t_command){0}, (t_command){0});
	if (!command_is_initialized(*out))
		return (E_OOM);
	err = reduce_pipeline(&productions->data[1], &out->conditional->first);
	if (err != NO_ERROR)
		return (err);
	return (reduce_complete_cmd_rest(&next_cmd_rest,
			&out->conditional->second));
}

size_t	n_connectors(const t_conditional *cond)
{
	size_t	n;

	n = 1;
	while (cond->second.type == CONDITIONAL_CMD)
	{
		cond = cond->second.conditional;
		n++;
	}
	return (n);
}

typedef struct s_conditional_cmd_data
{
	t_command					*commands;
	t_conditional_operator		*operators;
	size_t						n;
}								t_conditional_cmd_data;

t_conditional_cmd_data	cond_data_allocate(size_t n)
{
	t_command				*commands;
	t_conditional_operator	*operators;

	commands = ft_calloc(n + 1, sizeof(*commands));
	operators = ft_calloc(n, sizeof(*operators));
	if (commands == NULL || operators == NULL)
	{
		free(commands);
		free(operators);
		return ((t_conditional_cmd_data){0});
	}
	return ((t_conditional_cmd_data){.commands = commands,
		.operators = operators, .n = n});
}

t_conditional_cmd_data	gather_data_and_free(t_conditional *cond)
{
	t_conditional_cmd_data	data;
	t_conditional			*next;
	size_t					n;
	size_t					i;

	n = n_connectors(cond);
	data = cond_data_allocate(n);
	if (data.n == 0)
		return (data);
	i = 0;
	while (cond->second.type == CONDITIONAL_CMD)
	{
		data.commands[i] = cond->first;
		data.operators[i] = cond->op;
		i++;
		next = cond->second.conditional;
		free(cond);
		cond = next;
	}
	data.commands[i] = cond->first;
	data.operators[i] = cond->op;
	data.commands[i + 1] = cond->second;
	free(cond);
	return (data);
}

t_error	reconstruct_conditional_command(t_conditional_cmd_data data,
		t_conditional **out)
{
	t_conditional	*root;
	t_conditional	*new_root;
	t_command		lhs;

	root = conditional_new(data.operators[0], data.commands[0],
			data.commands[1]);
	if (!root)
		return (E_OOM);
	for (size_t i = 1; i < data.n; i++)
	{
		lhs = command_from_conditional(root);
		new_root = conditional_new(data.operators[i], lhs, data.commands[i
				+ 1]);
		if (!new_root)
		{
			*out = root;
			return (E_OOM);
		}
		root = new_root;
	}
	*out = root;
	return (NO_ERROR);
}

t_error	revert_conditional_associativity(t_conditional **out)
{
	t_conditional_cmd_data	data;

	data = gather_data_and_free(*out);
	*out = NULL;
	if (data.n == 0)
		return (E_OOM);
	return (reconstruct_conditional_command(data, out));
}

t_error	reduce_complete_command(t_symbol *root, t_command *out)
{
	t_error	err;

	assert(root->kind == COMPLETE_COMMAND);
	if (root->production->data[1].production->len == 0)
		return (reduce_pipeline(&root->production->data[0], out));
	out->conditional = conditional_new(0, (t_command){0}, (t_command){0});
	if (!out->conditional)
		return (E_OOM);
	out->conditional->op = operator_from_token_type(root->production->data[1].production->data[0].token.type);
	err = reduce_pipeline(&root->production->data[0], &out->conditional->first);
	if (err != NO_ERROR)
		return (err);
	err = reduce_complete_cmd_rest(&root->production->data[1],
			&out->conditional->second);
	if (err != NO_ERROR)
		return (err);
	revert_conditional_associativity(&out->conditional);
	*out = command_from_conditional(out->conditional);
	return (NO_ERROR);
}
