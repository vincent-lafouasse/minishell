#include "t_token_list.h"

void tkl_clear(t_token_list **tokens_p, t_destructor literal_destructor)
{
    if (!tokens_p)
        return;
    while (*tokens_p)
        tkl_delone(tokens_p, literal_destructor);
}
