/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:43:42 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:27:20 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "parse/t_command/t_command.h"
#include "productions/productions.h"
#include "reduction/reduction.h"
#include "t_symbol/t_symbol.h"
#include "tokenize/t_token.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "tokenize/tokenize.h"
#include <stdlib.h>

t_error	generate_parse_tree(const t_token_list *tokens, t_symbol *out)
{
	t_parser	state;

	state = parser_new(tokens);
	*out = produce_complete_command(&state);
	if (state.err != NO_ERROR)
	{
		symbol_clear(*out);
		return (state.err);
	}
	if (parser_peek(&state)->type != EOF_TOKEN)
	{
		symbol_clear(*out);
		return (E_UNEXPECTED_TOKEN);
	}
	return (NO_ERROR);
}

t_error	parse_tokens(const t_token_list *tokens, t_command *out)
{
	t_symbol	parse_tree;
	t_error		err;

	err = generate_parse_tree(tokens, &parse_tree);
	if (err != NO_ERROR)
		return (err);
	err = reduce_parse_tree_into_command(&parse_tree, out);
	if (err != NO_ERROR)
	{
		symbol_clear(parse_tree);
		command_destroy_and_clear(out);
		return (err);
	}
	symbol_clear(parse_tree);
	return (NO_ERROR);
}

t_error	parse(const char *input, t_command *out)
{
	t_token_list	*tokens;
	t_error			err;

	err = tokenize(input, &tokens);
	if (err != NO_ERROR)
		return (err);
	if (tokens->token.type == EOF_TOKEN)
	{
		tkl_clear(&tokens, free);
		*out = (t_command){0};
		return (NO_ERROR);
	}
	err = parse_tokens(tokens, out);
	if (err != NO_ERROR)
	{
		tkl_clear(&tokens, free);
		return (err);
	}
	tkl_clear(&tokens, free);
	return (NO_ERROR);
}
