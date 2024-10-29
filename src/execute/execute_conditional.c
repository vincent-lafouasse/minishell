#include "execute.h"
#include "parse/t_command/t_command.h"

#include <assert.h>
#include <sys/wait.h>

t_command_result execute_cond_or_simple(t_state* state, t_command cmd) { // bad temporary, should consider all command types
	assert(cmd.type == SIMPLE_CMD || cmd.type == CONDITIONAL_CMD);

	t_command_result cmd_res = (t_command_result){.error = NO_ERROR};

	if (cmd.type == SIMPLE_CMD) {
		t_simple* simple = cmd.simple;

		t_launch_result launch_res = launch_simple_command(state, simple, io_default(), NULL); // BAD need to check error member
		int options = 0;
		waitpid(launch_res.pids->pid, &cmd_res.status_code, options);

		return cmd_res;
	} else {
		return execute_conditional(state, cmd.conditional);
	}
}

t_command_result execute_conditional(t_state *state, t_conditional *cond) {
	t_conditional_operator op = cond->op;
	t_command first = cond->first;
	t_command second = cond->second;

	t_command_result first_res = execute_cond_or_simple(state, first); // bad, only takes simples for now

	int status = first_res.status_code;
	if (status != 0 && op == AND_OP) // false &&
		return first_res;
	if (status == 0 && op == OR_OP) //  true ||
		return first_res;

	return execute_cond_or_simple(state, second); // bad, only takes simples for now, all commands tbd
}
