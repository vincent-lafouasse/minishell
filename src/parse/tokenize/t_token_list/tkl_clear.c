#include "t_token_list.h"

void tkl_clear(t_token_list **tokens_p)
{
    if (!tokens_p)
        return;
    while (*tokens_p)
        tkl_delone(tokens_p);
}
