#include "lexer_internals.h"

#include "libft/ft_string.h"

t_error lexer_scan_string(t_lexer *lexer, t_token *out, char stop)
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
