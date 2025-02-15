/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_complete_command_rest.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:24 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:25:14 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

static void	set_oom(t_parser *state)
{
	state->err = E_OOM;
}

t_symbol	produce_complete_command_rest(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_COMPLETE_CMD_REST, 3);
	if (symbol.production == NULL)
		return (set_oom(state), symbol);
	if (parser_matches_one_of(state, (t_token_type[]){R_PAREN, EOF_TOKEN}, 2))
	{
	}
	else if (parser_matches_one_of(state, (t_token_type[]){AND_AND}, 1))
	{
		if (parser_accept_push(state, AND_AND, symbol.production))
			if (parser_produce_push(state, produce_pipeline, symbol.production))
				parser_produce_push(state, produce_complete_command_rest,
					symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){OR_OR}, 1))
	{
		if (parser_accept_push(state, OR_OR, symbol.production))
			if (parser_produce_push(state, produce_pipeline, symbol.production))
				parser_produce_push(state, produce_complete_command_rest,
					symbol.production);
	}
	else
		state->err = E_UNEXPECTED_TOKEN;
	return (symbol);
}
