#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <assert.h> // temporarily

t_command	reduce_subshell(t_symbol *subshell, t_symbol *trailing_redirs)
{
	t_subshell	*sub;
	t_error		err;

	assert (subshell->kind == SUBSHELL);
	assert (trailing_redirs->kind == SUBSHELL_PRECEDES);

	sub = subshell_new((t_command){0}, NULL);
	assert (sub != NULL);

	sub->cmd = reduce_complete_command(&subshell->production->data[1]);
	err = reduce_simple_command_like(trailing_redirs, NULL, &sub->redirections);
	if (err != NO_ERROR)
		assert("unimplemented");

	return command_from_subshell(sub);
}
