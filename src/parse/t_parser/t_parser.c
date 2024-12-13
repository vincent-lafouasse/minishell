/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_parser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:14:39 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:15:56 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/tokenize/t_token.h"
#include "t_parser.h"
#include <stdbool.h>
#include <stddef.h>

t_parser	parser_new(const t_token_list *tokens)
{
	return ((t_parser){.current = tokens, .err = NO_ERROR});
}

bool	parser_matches(const t_parser *parser, t_token_type type)
{
	return (parser_peek(parser)->type == type);
}

bool	parser_matches_one_of(const t_parser *parser,
		const t_token_type types[], size_t type_count)
{
	size_t	i;

	i = 0;
	while (i < type_count)
	{
		if (parser_matches(parser, types[i]))
			return (true);
		i++;
	}
	return (false);
}

const t_token	*parser_peek(const t_parser *parser)
{
	return (&parser->current->token);
}

t_error	parser_advance_copy(t_parser *parser, t_token *out)
{
	t_error	err;

	err = token_duplicate(parser->current->token, out);
	if (err != NO_ERROR)
		return (err);
	if (out->type != EOF_TOKEN)
		parser->current = parser->current->next;
	return (NO_ERROR);
}
