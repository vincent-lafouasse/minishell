#pragma once

#include "t_token_list/t_token_list.h"
#include <stddef.h>

typedef struct s_lexer
{
    const char *source;
    t_token_list *tokens;
    size_t src_len;
    size_t current;
    size_t start;
} t_lexer;

t_lexer lexer_init(const char *source);
void lexer_scan_token(t_lexer *lexer);
