#include "error/t_error.h"
#include "libft/ft_string.h"
#include "libft/string.h"
#include "t_lexer.h"
#include "tokenize/t_token.h"

#define SHELL_BREAK_CHARS "()<>;&| \t\n"
#define METACHARACTERS "()<>|&"

static t_error lexer_scan_metacharacter_token(t_lexer *lexer, t_token *out,
                                              char current);
static t_error lexer_scan_redirection_operators(t_lexer *lexer, t_token *out,
                                                char current);
static t_error lexer_scan_string(t_lexer *lexer, t_token *out, char stop);
static t_error lexer_scan_word(t_lexer *lexer, t_token *out);
static t_error fill_token(t_token token, t_token *out);

t_error lexer_scan_next_token(t_lexer *lexer, t_token *out)
{
    char c;

    c = lexer_advance(lexer);
    if (ft_strchr(METACHARACTERS, c) != NULL)
        return lexer_scan_metacharacter_token(lexer, out, c);
    if (c == '"' || c == '\'')
        return (lexer_scan_string(lexer, out, c));
    return (lexer_scan_word(lexer, out));
}

static t_error lexer_scan_metacharacter_token(t_lexer *lexer, t_token *out,
                                              char current)
{
    if (current == '(')
        return (fill_token((t_token){.type = L_PAREN}, out));
    if (current == ')')
        return (fill_token((t_token){.type = R_PAREN}, out));
    if (lexer_peek(lexer) == '|')
        return (lexer_advance(lexer),
                fill_token((t_token){.type = OR_OR}, out));
    else
        return (fill_token((t_token){.type = PIPE_TOKEN}, out));
    if (current == '&' && lexer_peek(lexer) == '&')
        return (lexer_advance(lexer),
                fill_token((t_token){.type = AND_AND}, out));
    if (current == '<' || current == '>')
        return (lexer_scan_redirection_operators(lexer, out, current));
    return E_UNREACHABLE;
}

static t_error lexer_scan_redirection_operators(t_lexer *lexer, t_token *out,
                                                char current)
{
    if (current == '<')
    {
        if (lexer_peek(lexer) == '<')
        {
            lexer_advance(lexer);
            return (fill_token((t_token){.type = DL_ANGLE_BRACKET}, out));
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
            lexer_advance(lexer);
            return (fill_token((t_token){.type = DR_ANGLE_BRACKET}, out));
        }
        else
        {
            return (fill_token((t_token){.type = R_ANGLE_BRACKET}, out));
        }
    }
    return (E_UNREACHABLE);
}

static t_error lexer_scan_string(t_lexer *lexer, t_token *out, char stop)
{
    char *literal;
    t_token_type token_type;
    size_t sz;

    while (lexer_peek(lexer) != stop && lexer->current < lexer->src_len)
        lexer_advance(lexer);
    if (lexer->current >= lexer->src_len)
        return (E_UNTERMINATED_QUOTE);
    lexer_advance(lexer);
    sz = lexer->current - lexer->start - 2;
    literal = ft_substr(lexer->source, lexer->start + 1, sz);
    token_type = SINGLE_QUOTE_STRING * (stop == '\'') +
                 DOUBLE_QUOTE_STRING * (stop == '"');
    return fill_token((t_token){.type = token_type, .literal = literal}, out);
}

static t_error lexer_scan_word(t_lexer *lexer, t_token *out)
{
    char *literal;
    size_t sz;

    while (ft_strchr(SHELL_BREAK_CHARS, lexer_peek(lexer)) == NULL &&
           lexer->current < lexer->src_len)
        lexer_advance(lexer);
    sz = lexer->current - lexer->start;
    literal = ft_substr(lexer->source, lexer->start, sz);
    return fill_token((t_token){.type = WORD, .literal = literal}, out);
}

static t_error fill_token(t_token token, t_token *out)
{
    if (!out)
        return (E_NULL_PARAM);
    *out = token;
    return (NO_ERROR);
}
