#include "log.h"

#include <stdio.h>

void log_token(t_token token) { printf("%s", token_repr(token)); }

void log_error(t_error err)
{
    if (err == NO_ERROR)
    {
        printf("No error\n");
        return;
    }
    printf("Error:\t%s\n", error_repr(err));
}
