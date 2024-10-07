#include "lexer_internals.h"
#include "libft/string.h"

static bool char_is_metachar(char c);

t_error lexer_scan_next_token(t_lexer *lexer, t_token *out)
{
    char c;
    t_error err;

    c = lexer_advance(lexer);
    if (char_is_metachar(c))
        err = lexer_scan_metacharacter_token(lexer, out, c);
    else
        err =  lexer_scan_word(lexer, out, c);
    lexer_skip_whitespace(lexer);
    return err;
}

static bool char_is_metachar(char c)
{
    return (ft_strchr(METACHARACTERS, c) != NULL);
}
