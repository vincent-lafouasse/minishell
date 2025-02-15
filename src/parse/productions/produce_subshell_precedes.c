/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_subshell_precedes.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:58 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:59 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_subshell_precedes(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_SUBSHELL_PRECEDES, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){DR_ANGLE_BRACKET,
			DL_ANGLE_BRACKET, L_ANGLE_BRACKET, R_ANGLE_BRACKET}, 4))
	{
		parser_produce_push(state, produce_redirect_list, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){PIPE, EOF_TOKEN,
			R_PAREN, OR_OR, AND_AND}, 5))
	{
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
