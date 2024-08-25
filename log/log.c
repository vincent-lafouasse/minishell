#include "log.h"

#include <stdio.h>

void log_token(t_token token)
{
	printf("%s", token_repr(token));
}
