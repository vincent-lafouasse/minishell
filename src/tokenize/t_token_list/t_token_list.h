#pragma once

#include "../t_token.h"
#include "error/t_error.h"

typedef struct s_token_list
{
    t_token token;
    struct s_token_list *next;
    struct s_token_list *prev;
} t_token_list;

t_token_list *tkl_new(t_token token);
void tkl_push_back_link(t_token_list **tokens_p, t_token_list *link);
t_error tkl_push_back(t_token_list **tokens_p, t_token token);
void tkl_clear(t_token_list **tokens_p);
