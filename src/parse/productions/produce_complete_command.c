/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_complete_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:20 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:21 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_complete_command(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_COMPLETE_CMD, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_produce_push(state, produce_pipeline, symbol.production))
		parser_produce_push(state, produce_complete_command_rest,
			symbol.production);
	return (symbol);
}
