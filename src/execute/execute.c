#include "execute.h"

#include "./builtin/builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h> // bad


t_command_result execute_command(t_state *state, t_command command) {

	t_command_result res;

	assert(state->our_children == NULL);
	if (command.type == CMD_SIMPLE)
		res = execute_simple_or_builtin(state, command.simple);
	else if (command.type == CMD_PIPELINE)
		res = execute_pipeline(state, command.pipeline);
	else if (command.type == CMD_CONDITIONAL)
		res = execute_conditional(state, command.conditional);
	else if (command.type == CMD_SUBSHELL)
		res = execute_subshell(state, command.subshell);
	else
		res = command_err(E_UNREACHABLE);

	return res;
}
