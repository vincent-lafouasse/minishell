#include "log.h"

#include <stdio.h>
#include <unistd.h>

void log_error(t_error err)
{
    printf("pid: %d\n", getpid());
    if (err == NO_ERROR)
    {
        printf("No error\n");
        return;
    }
    printf("Error:\t%s\n", error_repr(err));
}
