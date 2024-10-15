#include "parse/tokenize/t_token_list/t_token_list.h"
#include "reduction_internals.h"
#include "../tokenize/t_token.h"
#include "io/t_redir_list/t_redir_list.h"

#include <stdlib.h>
#include <assert.h> // temporarily

static t_redir_kind redir_kind_from_angle_bracket(t_token_type bracket)
{
	if (bracket == L_ANGLE_BRACKET)
		return (FROM_FILE);
	else if (bracket == R_ANGLE_BRACKET)
		return (INTO_FILE);
	else if (bracket == DL_ANGLE_BRACKET)
		return (HERE_DOCUMENT);
	else if (bracket == DR_ANGLE_BRACKET)
		return (APPEND_INTO_FILE);
	assert(false);
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

t_error reduce_simple_command_like(t_symbol *symbol, t_word_list **words, \
							t_redir_list **redirections)
{
	t_token_list	*head;
	t_token_list	*leaves;
	t_error			err;

	leaves = NULL;
	err = gather_leaves(symbol, &leaves);
	if (err != NO_ERROR)
		return err;
	head = leaves;
	while (leaves)
	{
		if (leaves->token.type == WORD)
		{
			err = wl_push_back(words, leaves->token.literal);
			if (err != NO_ERROR)
			{
				tkl_clear(&leaves, free);
				return E_OOM;
			}
			leaves = leaves->next;
		}
		else
		{
			err = rdl_push_back(redirections, redir_from_tokens(leaves->token, leaves->next->token));
			if (err != NO_ERROR)
			{
				tkl_clear(&leaves, free);
				return E_OOM;
			}
			leaves = leaves->next->next;
		}
	}
	tkl_clear(&head, NULL);
	return (NO_ERROR);
}
