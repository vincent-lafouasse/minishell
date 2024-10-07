#include "t_error.h"

#define NO_ERROR_REPR "No error"
#define UNRECOGNIZED_TOKEN_REPR "Unrecognized token"
#define UNEXPECTED_TOKEN_REPR "Unexpected token"
#define UNTERMINATED_QUOTE_REPR "Unterminated quote"
#define OUT_OF_MEMORY_REPR "Out of memory"
#define UNREACHABLE_REPR "Unreachable statement"
#define NULL_PARAM_REPR "Received NULL parameter"
#define DUMMY_REPR "Dummy error"

#define UNKNOWN_ERROR_REPR "Unknown error"

const char *error_repr(t_error err)
{
    if (err == NO_ERROR)
        return NO_ERROR_REPR;
    if (err == E_UNRECOGNIZED_TOKEN)
        return UNRECOGNIZED_TOKEN_REPR;
    if (err == E_UNEXPECTED_TOKEN)
        return UNEXPECTED_TOKEN_REPR;
    if (err == E_UNTERMINATED_QUOTE)
        return UNTERMINATED_QUOTE_REPR;
    if (err == E_OOM)
        return OUT_OF_MEMORY_REPR;
    if (err == E_UNREACHABLE)
        return UNREACHABLE_REPR;
    if (err == E_NULL_PARAM)
        return NULL_PARAM_REPR;
    if (err == E_DUMMY)
        return DUMMY_REPR;
    return UNKNOWN_ERROR_REPR;
}
