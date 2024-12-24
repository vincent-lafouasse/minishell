#pragma once

#include <stdbool.h>

/* Update error_repr.c when adding a new variant */
typedef enum e_error
{
    NO_ERROR,
    E_UNRECOGNIZED_TOKEN,
    E_UNEXPECTED_TOKEN,
    E_UNTERMINATED_QUOTE,
    E_OOM,
    E_UNREACHABLE,
    E_NULL_PARAM,
    E_MALFORMED_ENVP,
    E_COMMAND_NOT_FOUND,
    E_FORK,
    E_DUP2,
    E_EXECVE,
    E_ACCESS,
    E_GETCWD,
    E_WRITE,
    E_WAIT,
    E_OPEN,
    E_UNLINK,
    E_DUMMY,
    E_AMBIGUOUS_REDIRECT,
    E_INTERRUPTED, // XXX this does not refer to the errno value `EINTR`, rename?
} t_error;

const char *error_repr(t_error err);
char parse_error_exit_code(t_error err);
void die(const char *msg);
void ft_assert(bool assertion, const char *msg);
