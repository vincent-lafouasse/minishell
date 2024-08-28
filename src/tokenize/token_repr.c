#include "t_token.h"

#define DUMMY_TOKEN_REPR "that's a token for sure"

const char	*token_repr(t_token token)
{
	(void)token;
	return (DUMMY_TOKEN_REPR);
}
