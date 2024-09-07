#include "lexer_internals.h"
#include "libft/string.h"

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
