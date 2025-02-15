/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   produce_here_end.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:18:28 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:18:28 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "productions.h"

t_symbol	produce_here_end(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_HERE_END, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	parser_accept_push(state, WORD, symbol.production);
	return (symbol);
}
