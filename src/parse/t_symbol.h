#pragma once

#include "tokenize/t_token.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct s_symbol_array t_symbol_array;
enum e_symbol_kind : int;

typedef struct s_symbol
{
    // either T or NT,
    // if non terminal: contains right_hand_side and no token
    // if terminal: no right_hand_side and has token
    enum e_symbol_kind kind;
    union
    {
        t_symbol_array *production;
        t_token token;
    };
} t_symbol;

typedef enum e_symbol_kind : int
{
    TERMINAL,
    EXPR,
    EXPR_REST,
    TERM,
    TERM_REST,
    FACTOR
} t_symbol_kind;

typedef struct s_symbol_array
{
    size_t len;
    size_t cap;
    struct s_symbol data[];
} t_symbol_array;

t_symbol_array *symbol_array_with_cap(size_t cap);
bool symbol_array_push(t_symbol_array *this, t_symbol symbol);

t_symbol symbol_new_non_terminal(t_symbol_kind kind, size_t capacity);
t_symbol symbol_new_terminal(t_token token);

void *symbol_clear(t_symbol symbol);
