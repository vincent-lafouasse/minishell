#pragma once

typedef enum e_token_type
{
    WORD,
    PIPE,
    L_ANGLE_BRACKET,
    DL_ANGLE_BRACKET,
    R_ANGLE_BRACKET,
    DR_ANGLE_BRACKET,
    L_PAREN,
    R_PAREN,
    AND_AND,
    OR_OR,
    DOUBLE_QUOTE_STRING,
    SINGLE_QUOTE_STRING,
    EOF_TOKEN,
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *literal;
} t_token;

const char *token_repr(t_token token);