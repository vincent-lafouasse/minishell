#include "lexer_internals.h"
#include "libft/string.h"

#define WHITESPACE " "

void    lexer_skip_whitespace(t_lexer* lexer)
{
    while (lexer->current < lexer->src_len &&
           ft_strchr(WHITESPACE, lexer_peek(lexer)) != NULL)
        lexer_advance(lexer);
}
