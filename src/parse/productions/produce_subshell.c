/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:56 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:56 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_subshell(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_SUBSHELL, 3);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_accept_push(state, L_PAREN, symbol.production))
		if (parser_produce_push(state, produce_complete_command,
				symbol.production))
			parser_accept_push(state, R_PAREN, symbol.production);
	return (symbol);
}
