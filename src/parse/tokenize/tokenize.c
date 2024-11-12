/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:53:09 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:53:10 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_lexer/t_lexer.h"
#include "tokenize.h"
#include <stdlib.h>

static t_error	cleanup_propagate(t_lexer *lexer, t_error err);

t_error	tokenize(const char *input, t_token_list **out)
{
	t_lexer	lexer;
	t_token	token;
	t_error	err;

	lexer = lexer_init(input);
	lexer_skip_whitespace(&lexer);
	while (lexer.current < lexer.src_len)
	{
		lexer.start = lexer.current;
		err = lexer_scan_next_token(&lexer, &token);
		if (err != NO_ERROR)
			return (cleanup_propagate(&lexer, err));
		err = tkl_push_back(&lexer.tokens, token);
		if (err != NO_ERROR)
			return (cleanup_propagate(&lexer, err));
		lexer_skip_whitespace(&lexer);
	}
	err = tkl_push_back(&lexer.tokens, (t_token){.type = EOF_TOKEN});
	if (err != NO_ERROR)
		return (cleanup_propagate(&lexer, err));
	*out = lexer.tokens;
	return (NO_ERROR);
}

static t_error	cleanup_propagate(t_lexer *lexer, t_error err)
{
	tkl_clear(&lexer->tokens, free);
	return (err);
}
