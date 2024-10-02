#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <stddef.h> // temporarily
#include <stdlib.h> // temporarily
#include <assert.h> // temporarily

t_command	reduce_simple_command(t_symbol *root)
{
	t_simple		*simple;

	assert (root->kind == SIMPLE_COMMAND);
	simple = malloc(sizeof(*simple));
	assert (simple != NULL);
	*simple = (t_simple){0};
	reduce_simple_command_like(root, &simple->words, &simple->redirections);
	return (t_command){.type = SIMPLE_CMD, .simple = simple};
}
