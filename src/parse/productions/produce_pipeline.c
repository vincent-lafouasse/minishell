/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:39 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:40 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_pipeline(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_PIPELINE, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_produce_push(state, produce_command, symbol.production))
		parser_produce_push(state, produce_pipeline_rest, symbol.production);
	return (symbol);
}
