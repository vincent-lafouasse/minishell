#include "error/t_error.h"
#include "t_lexer.h"
#include "libft/ft_string.h"

t_error	lexer_scan_double_quote_string(t_lexer *lexer, t_token *out)
{
	char* literal;

	while (lexer_peek(lexer) != '"' && lexer->current < lexer->src_len)
	{
		lexer_advance(lexer);
	}
	if (lexer->current >= lexer->src_len)
		return E_UNTERMINATED_QUOTE;
	lexer_advance(lexer);
	literal = ft_substr(lexer->source, lexer->start + 1, lexer->start - ;lexer->current - 2);
	*out = (t_token){.type = DOUBLE_QUOTE_STRING, .literal = literal};
	return NO_ERROR;
}

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
	if (c == '>')
	{
		if (lexer_peek(lexer) == '>')
		{
			*out = (t_token){.type = DR_ANGLE_BRACKET};
			lexer->current++;
			return (NO_ERROR);
		}
		else
		{
			*out = (t_token){.type = R_ANGLE_BRACKET};
			return (NO_ERROR);
		}
	}
	if (c == '&' && lexer_peek(lexer) == '&')
	{
		*out = (t_token){.type = AND_AND};
		lexer->current++;
		return (NO_ERROR);
	}
	if (c == '|')
	{
		if (lexer_peek(lexer) == '|')
		{
			*out = (t_token){.type = OR_OR};
			lexer->current++;
			return (NO_ERROR);
		}
		else
		{
			*out = (t_token){.type = PIPE_TOKEN};
			return (NO_ERROR);
		}
	}
	return (E_UNRECOGNIZED_TOKEN);
}
