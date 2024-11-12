/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:54 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:20:26 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_simple_command(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_SIMPLE_CMD, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){R_ANGLE_BRACKET,
			DR_ANGLE_BRACKET, DL_ANGLE_BRACKET, L_ANGLE_BRACKET}, 4))
	{
		if (parser_produce_push(state, produce_cmd_prefix, symbol.production))
			parser_produce_push(state, produce_cmd_prefix_precedes,
				symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){WORD}, 1))
	{
		if (parser_accept_push(state, WORD, symbol.production))
			parser_produce_push(state, produce_maybe_cmd_suffix,
				symbol.production);
	}
	else
		state->err = E_UNEXPECTED_TOKEN;
	return (symbol);
}
