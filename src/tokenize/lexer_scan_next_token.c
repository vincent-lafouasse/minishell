#include "error/t_error.h"
#include "t_lexer.h"

static char advance(t_lexer* lexer)
{
	
	return lexer->source[lexer->current++];
}

static char peek(t_lexer* lexer)
{
	return lexer->source[lexer->current];
}

t_error	lexer_scan_next_token(t_lexer *lexer, t_token *out)
{
	char c = advance(lexer);

	if (c == '(')
	{
		*out = (t_token){.type = L_PAREN};
		return (NO_ERROR);
	}
	if (c == ')')
	{
		*out = (t_token){.type = R_PAREN};
		return (NO_ERROR);
	}
	if (c == '<')
	{
		if (peek(lexer) == '<')
		{
			*out = (t_token){.type = DL_ANGLE_BRACKET};
			lexer->current++;
			return (NO_ERROR);
		}
		else
		{
			*out = (t_token){.type = L_ANGLE_BRACKET};
			return (NO_ERROR);
		}
	}
	return (E_UNRECOGNIZED_TOKEN);
}
