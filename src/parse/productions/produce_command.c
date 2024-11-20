/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:19 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:20 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_command(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_COMMAND, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){DL_ANGLE_BRACKET,
			L_ANGLE_BRACKET, WORD, DR_ANGLE_BRACKET, R_ANGLE_BRACKET}, 5))
	{
		parser_produce_push(state, produce_simple_command, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){L_PAREN}, 1))
	{
		if (parser_produce_push(state, produce_subshell, symbol.production))
			parser_produce_push(state, produce_subshell_precedes,
				symbol.production);
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
