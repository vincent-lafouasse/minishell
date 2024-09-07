#include "lexer_internals.h"

t_error fill_token(t_token token, t_token *out)
{
    if (!out)
        return (E_NULL_PARAM);
    *out = token;
    return (NO_ERROR);
}
