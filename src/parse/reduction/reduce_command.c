#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <assert.h> // temporarily

// command : simple_command
//         | subshell redirect_list
//         | subshell
t_error	reduce_command(t_symbol *root, t_command *out)
{
	t_error err;

	assert (root->kind == COMMAND);
	if (root->production->data[0].kind == SIMPLE_COMMAND)
		err = reduce_simple_command(&root->production->data[0], out);
	else
		err = reduce_subshell(&root->production->data[0], \
							   &root->production->data[1], out);
	return err;
}
