#include "execute.h"
#include "parse/t_command/t_command.h"

#include <assert.h>
#include <sys/wait.h>

t_command_result execute_conditional(t_state *state, t_conditional *cond) {
	t_command_result cmd_res = (t_command_result){.error = NO_ERROR};
	t_command command = command_from_conditional(cond);

	while (command.type == CONDITIONAL_CMD) {
		assert (command.conditional->first.type == SIMPLE_CMD);
		t_simple* simple = command.conditional->first.simple;

		t_launch_result launch_res = launch_simple_command(state, simple, io_default(), NULL); // BAD need to check error member
		assert (launch_res.pids != NULL);

		int options = 0;
		waitpid(launch_res.pids->pid, &cmd_res.status_code, options);

		t_conditional_operator op = command.conditional->op;

		int status = cmd_res.status_code;
		if (status != 0 && op == AND_OP)
			return cmd_res;
		if (status == 0 && op == OR_OP)
			return cmd_res;

		command = command.conditional->second;
	}
	assert (command.type == SIMPLE_CMD);
	t_launch_result launch_res = launch_simple_command(state, command.simple, io_default(), NULL); // BAD need to check error member
	int options = 0;
	waitpid(launch_res.pids->pid, &cmd_res.status_code, options);

	return cmd_res;
}

