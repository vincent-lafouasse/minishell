/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_program.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:43 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:47 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_program(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_PROGRAM, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	parser_produce_push(state, produce_complete_command, symbol.production);
	return (symbol);
}
