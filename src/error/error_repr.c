#include "t_error.h"

#define NO_ERROR_REPR "No error"
#define UNRECOGNIZED_TOKEN_REPR "Unrecognised token"
#define UNTERMINATED_QUOTE_REPR "Unterminated quote"

#define UNKNOWN_ERROR_REPR "Unknown error"

const char* error_repr(t_error err)
{
	if (err == NO_ERROR)
		return NO_ERROR_REPR;
	if (err == E_UNRECOGNIZED_TOKEN)
		return UNRECOGNIZED_TOKEN_REPR;
	if (err == E_UNTERMINATED_QUOTE)
		return UNTERMINATED_QUOTE_REPR;
	return UNKNOWN_ERROR_REPR;
}
