/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_cmd_suffix.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:14 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:15 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_cmd_suffix(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_CMD_SUFFIX, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){WORD}, 1))
	{
		if (parser_accept_push(state, WORD, symbol.production))
			parser_produce_push(state, produce_cmd_suffix_rest,
				symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){DL_ANGLE_BRACKET,
			R_ANGLE_BRACKET, DR_ANGLE_BRACKET, L_ANGLE_BRACKET}, 4))
	{
		if (parser_produce_push(state, produce_io_redirect, symbol.production))
			parser_produce_push(state, produce_cmd_suffix_rest,
				symbol.production);
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
