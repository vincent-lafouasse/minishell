#include "t_token_list.h"

#include <stdlib.h>

void tkl_delone(t_token_list **tokens_p)
{
    t_token_list *buffer;

    if (!tokens_p || !*tokens_p)
        return;
    buffer = (*tokens_p)->next;
    free((*tokens_p)->token.literal);
    free(*tokens_p);
    *tokens_p = buffer;
    if (buffer)
        buffer->prev = NULL;
    return;
}
