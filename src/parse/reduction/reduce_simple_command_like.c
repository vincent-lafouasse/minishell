/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce_simple_command_like.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:48 by poss              #+#    #+#             */
/*   Updated: 2025/02/01 22:02:31 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tokenize/t_token.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include "parse/tokenize/t_token_list/t_token_list.h"
#include "reduction_internals.h"
#include <stdlib.h>

static t_redir_kind	redir_kind_from_angle_bracket(t_token_type bracket)
{
	if (bracket == L_ANGLE_BRACKET)
		return (FROM_FILE);
	else if (bracket == R_ANGLE_BRACKET)
		return (INTO_FILE);
	else if (bracket == DL_ANGLE_BRACKET)
		return (HERE_DOCUMENT);
	else if (bracket == DR_ANGLE_BRACKET)
		return (APPEND_INTO_FILE);
	return (0);
}

static t_redirect	redir_from_tokens(t_token bracket, t_token word)
{
	t_redirect	redir;

	redir = (t_redirect){0};
	redir.kind = redir_kind_from_angle_bracket(bracket.type);
	if (redir.kind == HERE_DOCUMENT)
		redir.doc = (t_here_doc){NULL, word.literal};
	else
		redir.filename = word.literal;
	return (redir);
}

t_error	process_word_or_redir(t_token_list ***leaves, t_word_list **words,
		t_redir_list **redirections)
{
	t_error			err;
	t_token_list	*current;

	current = **leaves;
	if (current->token.type == WORD)
	{
		err = wl_push_back(words, current->token.literal);
		if (err != NO_ERROR)
		{
			return (E_OOM);
		}
		*leaves = &current->next;
	}
	else
	{
		err = rdl_push_back(redirections, redir_from_tokens(current->token,
					current->next->token));
		if (err != NO_ERROR)
		{
			return (E_OOM);
		}
		*leaves = &current->next->next;
	}
	return (NO_ERROR);
}

t_error	reduce_simple_command_like(t_symbol *symbol, t_word_list **words,
		t_redir_list **redirections)
{
	t_token_list	*leaves;
	t_token_list	**current;
	t_error			err;

	leaves = NULL;
	err = gather_leaves(symbol, &leaves);
	if (err != NO_ERROR)
		return (err);
	current = &leaves;
	while (*current)
	{
		err = process_word_or_redir(&current, words, redirections);
		if (err != NO_ERROR)
		{
			tkl_clear(current, free);
			tkl_clear(&leaves, NULL);
			return (err);
		}
	}
	tkl_clear(&leaves, NULL);
	return (NO_ERROR);
}
