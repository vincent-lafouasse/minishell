#pragma once

#include <stdbool.h>

typedef enum e_error
{
    NO_ERROR,
    E_UNRECOGNIZED_TOKEN,
    E_UNTERMINATED_QUOTE,
    E_DUMMY,
} t_error;

const char *error_repr(t_error err);
void die(const char *msg);
void ft_assert(bool assertion, const char *msg);
