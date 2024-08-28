#include "error/t_error.h"
#include "t_lexer.h"


t_error	lexer_scan_next_token(t_lexer *lexer, t_token *out)
{
	char c = lexer->source[lexer->current++];

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
	return (E_UNRECOGNIZED_TOKEN);
}
