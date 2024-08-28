#include "error/t_error.h"
#include "t_lexer.h"

t_error	lexer_scan_next_token(t_lexer *lexer, t_token *out)
{
	char c = lexer_advance(lexer);

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
		if (lexer_peek(lexer) == '<')
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
