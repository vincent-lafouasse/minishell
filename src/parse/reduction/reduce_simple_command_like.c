#include "reduction_internals.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include "tokenize/t_token.h"

#include <assert.h>

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
	assert(true);
	return (0);
}

void reduce_simple_command_like(t_symbol *symbol, t_word_list **words, \
							t_redir_list **redirections)
{
	t_token_list	*leaves;

	leaves = gather_leaves(symbol);
	while (leaves)
	{
		if (leaves->token.type == WORD)
		{
			wl_push_back(words, leaves->token.literal);
			leaves = leaves->next;
		}
		else
		{
			t_redirect	redir;
			t_token		bracket;
			t_token		word;

			bracket = leaves->token;
			word = leaves->next->token;
			redir.kind = redir_kind_from_angle_bracket(bracket.type);
			if (redir.kind == HERE_DOCUMENT)
				redir.doc = (t_here_doc){NULL, word.literal};
			else
				redir.filename = word.literal;
			rdl_push_back(redirections, redir);
			leaves = leaves->next->next;
		}
	}
}
