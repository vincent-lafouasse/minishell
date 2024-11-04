#include "execute.h"
#include "parse/t_command/t_command.h"

#include <assert.h>
#include <sys/wait.h>

t_command_result execute_conditional(t_state *state, t_conditional *cond) {
	t_conditional_operator op = cond->op;
	t_command first = cond->first;
	t_command second = cond->second;

	t_command_result first_res = execute_command(state, first); // bad, may err

	int status = first_res.status_code;
	if (status != 0 && op == AND_OP) // false &&
		return first_res;
	if (status == 0 && op == OR_OP) //  true ||
		return first_res;

	return execute_command(state, second);
}
