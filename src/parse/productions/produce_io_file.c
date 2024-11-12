/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_io_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:29 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:24:35 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

static void	set_oom(t_parser *state)
{
	state->err = E_OOM;
}

t_symbol	produce_io_file(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_IO_FILE, 2);
	if (symbol.production == NULL)
		return (set_oom(state), symbol);
	if (parser_matches_one_of(state, (t_token_type[]){DR_ANGLE_BRACKET}, 1))
	{
		if (parser_accept_push(state, DR_ANGLE_BRACKET, symbol.production))
			parser_produce_push(state, produce_filename, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){L_ANGLE_BRACKET}, 1))
	{
		if (parser_accept_push(state, L_ANGLE_BRACKET, symbol.production))
			parser_produce_push(state, produce_filename, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){R_ANGLE_BRACKET}, 1))
	{
		if (parser_accept_push(state, R_ANGLE_BRACKET, symbol.production))
			parser_produce_push(state, produce_filename, symbol.production);
	}
	else
		state->err = E_UNEXPECTED_TOKEN;
	return (symbol);
}
