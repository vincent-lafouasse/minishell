#pragma once

#include "../t_token.h"

typedef struct s_token_list
{
    t_token token;
    struct s_token_list* next;
    struct s_token_list* prev;
} t_token_list;
