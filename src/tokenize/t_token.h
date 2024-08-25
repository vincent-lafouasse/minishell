#pragma once

typedef enum e_token_type
{
    IDENTIFIER,
    PIPE_TOKEN,
    REDIRECT_INPUT,
    REDIRECT_OUTPUT,
    APPEND_TO_FILE,
    REDIRECT_FROM_HERE_DOC,
    SEPARATOR_TOKEN,
    AND_AND,
    OR_OR,
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *literal;
} t_token;

const char *token_repr(t_token token);
