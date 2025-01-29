#include "shell.h"

#include "parse/t_command/t_command.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

__attribute__((noreturn)) void shell_exit(t_state *state, int exit_status)
{
	shell_cleanup(state);
	exit(exit_status);
}

void shell_cleanup(t_state *state)
{
	command_destroy_and_clear(&state->root);
	free(state->line);
	rl_clear_history();
	env_destroy(&state->env);
	pidl_clear(&state->our_children);
}
