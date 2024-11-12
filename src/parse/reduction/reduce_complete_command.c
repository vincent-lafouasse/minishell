/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce_complete_command.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:41 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 19:47:58 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"
#include "../tokenize/t_token.h"
#include "error/t_error.h"
#include "reduction_internals.h"
#include <assert.h> // temporarily
#include <stdbool.h>

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
	t_conditional_operator	op;
	t_error					err;

	assert(cmd_rest->kind == SYM_COMPLETE_CMD_REST);
	productions = cmd_rest->production;
	next_cmd_rest = productions->data[2];
	if (next_cmd_rest.production->len == 0)
		return (reduce_pipeline(&productions->data[1], out));
	op = operator_from_token_type(
			next_cmd_rest.production->data[0].token.type);
	*out = command_new_conditional(op, (t_command){0}, (t_command){0});
	if (!command_is_initialized(*out))
		return (E_OOM);
	err = reduce_pipeline(&productions->data[1], &out->conditional->first);
	if (err != NO_ERROR)
		return (err);
	return (reduce_complete_cmd_rest(&next_cmd_rest,
			&out->conditional->second));
}

t_error	reduce_complete_command(t_symbol *root, t_command *out)
{
	t_error	err;

	assert(root->kind == SYM_COMPLETE_CMD);
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
	err = invert_conditional_associativity(&out->conditional);
	if (err != NO_ERROR)
		return (err);
	*out = command_from_conditional(out->conditional);
	return (err);
}
