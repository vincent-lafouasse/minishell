#include "t_lexer.h"

char lexer_advance(t_lexer *lexer) {
    if (lexer->current < lexer->src_len)
        return lexer->source[lexer->current++];
    else
        return '\0';
}

char lexer_peek(t_lexer *lexer) {
    if (lexer->current <= lexer->src_len)
        return lexer->source[lexer->current];
    else
        return '\0';
}
