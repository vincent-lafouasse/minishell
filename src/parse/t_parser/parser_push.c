/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_push.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:15:50 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:16:37 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/tokenize/t_token.h"
#include "t_parser.h"

__attribute__((warn_unused_result)) bool	parser_produce_push(
	t_parser *parser,
	t_symbol (*prod)(t_parser *),
	t_symbol_array *out)
{
	t_symbol	symbol;

	symbol = prod(parser);
	symbol_array_push(out, symbol);
	return (parser->err == NO_ERROR);
}

__attribute__((warn_unused_result)) bool	parser_accept_push(t_parser *parser,
	t_token_type type, t_symbol_array *out)
{
	bool		matches;
	t_symbol	symbol;
	t_token		token;

	matches = parser_matches(parser, type);
	if (!matches)
	{
		parser->err = E_UNEXPECTED_TOKEN;
		return (false);
	}
	parser->err = parser_advance_copy(parser, &token);
	if (parser->err != NO_ERROR)
		return (false);
	symbol = symbol_new_terminal(token);
	symbol_array_push(out, symbol);
	return (true);
}
