#include "../builtin.h"

#include "execute/execute.h"

t_command_result do_assignments(t_env **env, t_word_list *assignments);
t_command_result print_all_variables(t_state *state);

t_command_result execute_export(t_state *state, t_simple *builtin)
{
	t_word_list *assignments;

	assignments = builtin->words->next;

	if (assignments)
		return do_assignments(&state->env, assignments);
	else
		return print_all_variables(state);
}
