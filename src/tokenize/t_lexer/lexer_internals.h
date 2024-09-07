#pragma once

#include "t_lexer.h"

t_error fill_token(t_token token, t_token *out);

char lexer_advance(t_lexer *lexer);
char lexer_peek(t_lexer *lexer);
