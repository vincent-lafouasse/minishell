/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_pipeline_rest.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:41 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:42 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_pipeline_rest(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_PIPELINE_REST, 3);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){PIPE}, 1))
	{
		if (parser_accept_push(state, PIPE, symbol.production))
			if (parser_produce_push(state, produce_command, symbol.production))
				parser_produce_push(state, produce_pipeline_rest,
					symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){R_PAREN, OR_OR,
			AND_AND, EOF_TOKEN}, 4))
	{
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
