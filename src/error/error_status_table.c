#include "t_error.h"

#include <stdlib.h>

char error_status_table(t_error err) {
    if (err == NO_ERROR)
        return 0;
    if (err == E_UNEXPECTED_TOKEN)
        return 2;
    if (err == E_COMMAND_NOT_FOUND)
        return 127;
    return EXIT_FAILURE;
}
