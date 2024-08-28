#include "log.h"

#include <stdio.h>

void log_token(t_token token)
{
	printf("%s", token_repr(token));
	if (token.literal)
		printf("\t%s", token.literal);
}
