#include "execute.h"
#include "parse/t_command/t_command.h"

#include <assert.h>
#include <sys/wait.h>

t_command_result execute_conditional(t_state *state, t_conditional *cond) {
	t_command_result first_res;
	int status;

	first_res = execute_command(state, cond->first);
	if (first_res.error != NO_ERROR)
		return (first_res);
	status = first_res.status_code;
	if (status != 0 && cond->op == AND_OP) // false &&
		return first_res;
	if (status == 0 && cond->op == OR_OP) //  true ||
		return first_res;

	return execute_command(state, cond->second);
}
