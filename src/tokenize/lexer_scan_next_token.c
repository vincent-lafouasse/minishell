#include "error/t_error.h"
#include "libft/ft_string.h"
#include "t_lexer.h"
#include "tokenize/t_token.h"

static t_error lexer_scan_redirection_operators(t_lexer* lexer, t_token* out, char current);
static t_error	lexer_scan_pipe_oror(t_lexer *lexer, t_token *out);
static t_error	lexer_scan_single_quote_string(t_lexer *lexer, t_token *out);
static t_error	lexer_scan_double_quote_string(t_lexer *lexer, t_token *out);
static t_error	fill_token(t_token token, t_token *out);

t_error	lexer_scan_next_token(t_lexer *lexer, t_token *out)
{
	char	c;

	c = lexer_advance(lexer);
	if (c == '(')
		return (fill_token((t_token){.type = L_PAREN}, out));
	if (c == ')')
		return (fill_token((t_token){.type = R_PAREN}, out));
	if (c == '<' || c == '>')
		return lexer_scan_redirection_operators(lexer, out, c);
	if (c == '&' && lexer_peek(lexer) == '&')
		return (lexer->current++, fill_token((t_token){.type = AND_AND}, out));
	if (c == '|')
		return lexer_scan_pipe_oror(lexer, out);
	if (c == '"')
		return (lexer_scan_double_quote_string(lexer, out));
	if (c == '\'')
		return (lexer_scan_single_quote_string(lexer, out));
	return (E_UNRECOGNIZED_TOKEN);
}

static t_error lexer_scan_redirection_operators(t_lexer* lexer, t_token* out, char current)
{
	if (current == '<')
	{
		if (lexer_peek(lexer) == '<')
		{
			lexer->current++;
			return fill_token((t_token){.type = DL_ANGLE_BRACKET}, out);
		}
		else
		{
			return (fill_token((t_token){.type = L_ANGLE_BRACKET}, out));
		}
	}
	if (current == '>')
	{
		if (lexer_peek(lexer) == '>')
		{
			lexer->current++;
			return fill_token((t_token){.type = DR_ANGLE_BRACKET}, out);
		}
		else
		{
			return (fill_token((t_token){.type = R_ANGLE_BRACKET}, out));
		}
	}
	return E_UNREACHABLE;
}

static t_error	lexer_scan_pipe_oror(t_lexer *lexer, t_token *out)
{
	if (lexer_peek(lexer) == '|')
	{
		lexer->current++;
		return fill_token((t_token){.type = OR_OR}, out);
	}
	else
	{
		return (fill_token((t_token){.type = PIPE_TOKEN}, out));
	}
	return E_UNREACHABLE;
}

static t_error	lexer_scan_single_quote_string(t_lexer *lexer, t_token *out)
{
	char	*literal;
	size_t	sz;

	while (lexer_peek(lexer) != '\'' && lexer->current < lexer->src_len)
	{
		lexer_advance(lexer);
	}
	if (lexer->current >= lexer->src_len)
		return (E_UNTERMINATED_QUOTE);
	lexer_advance(lexer);
	sz = (size_t)(lexer->current - lexer->start) - 2;
	literal = ft_substr(lexer->source, lexer->start + 1, sz);
	*out = (t_token){.type = SINGLE_QUOTE_STRING, .literal = literal};
	return (NO_ERROR);
}

static t_error	lexer_scan_double_quote_string(t_lexer *lexer, t_token *out)
{
	char	*literal;
	size_t	sz;

	while (lexer_peek(lexer) != '"' && lexer->current < lexer->src_len)
	{
		lexer_advance(lexer);
	}
	if (lexer->current >= lexer->src_len)
		return (E_UNTERMINATED_QUOTE);
	lexer_advance(lexer);
	sz = (size_t)(lexer->current - lexer->start) - 2;
	literal = ft_substr(lexer->source, lexer->start + 1, sz);
	*out = (t_token){.type = DOUBLE_QUOTE_STRING, .literal = literal};
	return (NO_ERROR);
}

static t_error	fill_token(t_token token, t_token *out)
{
	if (!out)
		return (E_NULL_PARAM);
	*out = token;
	return NO_ERROR;
}
