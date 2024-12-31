#include "t_error.h"

#include <stdbool.h>

bool is_fatal_error(t_error err)
{
	return (err == E_OOM);
}
