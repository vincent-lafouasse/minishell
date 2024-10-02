#include "log.h"

#include <stdio.h>

void log_token(t_token token)
{
    printf("%s", token_repr(token));
    if (token.literal)
        printf("\t%s", token.literal);
}

void log_token_list(const t_token_list *tkl)
{
    while (tkl)
    {
        printf("token:\t");
        log_token(tkl->token);
        printf("\n");
        tkl = tkl->next;
    }
}

void log_error(t_error err)
{
    if (err == NO_ERROR)
    {
        printf("No error\n");
        return;
    }
    printf("Error:\t%s\n", error_repr(err));
}

void log_word_list(const t_word_list *words)
{
    while (words)
    {
        printf(" > %s", words->contents);
        printf("\n");
        words = words->next;
    }
}
