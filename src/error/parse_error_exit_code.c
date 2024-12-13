#include "t_error.h"

#include <stdlib.h>

char parse_error_exit_code(t_error err) {
    if (err == E_UNEXPECTED_TOKEN)
        return 2;
    return EXIT_FAILURE;
}
