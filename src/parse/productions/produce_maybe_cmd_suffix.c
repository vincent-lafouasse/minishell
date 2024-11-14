/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_maybe_cmd_suffix.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:37 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:38 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_maybe_cmd_suffix(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_MAYBE_CMD_SUFFIX, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){R_ANGLE_BRACKET,
			L_ANGLE_BRACKET, DL_ANGLE_BRACKET, DR_ANGLE_BRACKET, WORD}, 5))
	{
		parser_produce_push(state, produce_cmd_suffix, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){R_PAREN, PIPE,
			EOF_TOKEN, OR_OR, AND_AND}, 5))
	{
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
