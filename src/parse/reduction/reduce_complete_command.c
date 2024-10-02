#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <stdbool.h>

#include <stdlib.h> // temporarily
#include <assert.h> // temporarily

static bool recurse(t_command *out, t_symbol *complete_cmd_rest);
static t_command	reduce_complete_cmd_rest(t_symbol *symbol);

t_command	reduce_complete_command(t_symbol *root)
{
	(void)root;
	return (t_command){0};
}
