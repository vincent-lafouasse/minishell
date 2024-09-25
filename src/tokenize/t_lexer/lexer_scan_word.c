#include "lexer_internals.h"

#include "libft/ft_string.h"
#include "libft/string.h"

t_error lexer_scan_word(t_lexer *lexer, t_token *out)
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
