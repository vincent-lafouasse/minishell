#pragma once

#include "error/t_error.h"
#include "../t_token_list/t_token_list.h"
#include <stddef.h>

typedef enum e_lexer_state
{
    AWAITING_IDENTIFIER,
    AWAITING_WORD,
} t_lexer_state;

typedef struct s_lexer
{
    const char *source;
    t_token_list *tokens;
    size_t src_len;
    size_t current;
    size_t start;
    t_lexer_state state;
} t_lexer;

t_lexer lexer_init(const char *source);
t_error lexer_scan_next_token(t_lexer *lexer, t_token *out);

char lexer_advance(t_lexer *lexer);
char lexer_peek(t_lexer *lexer);
