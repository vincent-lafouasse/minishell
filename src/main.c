#include <stdlib.h>
#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "error/t_error.h"
#include "execute/execute.h"
#include "execute/t_env/t_env.h"
#include "log/log.h" // todo remove in production mode
#include "parse/parse.h"

#define SHELL_PROMPT "minishell$ "

t_error run_command(char* input, t_state* state)
{
	t_error err;
	t_command cmd;

	// TODO: double check that bash really does behaves like this
	if (*input == '\0')
	{
		free(input);
		return NO_ERROR;
	}
	add_history(input);

	err = parse(input, &cmd);
	if (err != NO_ERROR)
	{
		free(input);
		return err;
	}

	if (cmd.type == SIMPLE_CMD)
	{
		t_command_result res;
		res = execute_simple_command(state, cmd.simple, (t_io){0, 1});
		log_error(res.error);

		wait(NULL);
	}
	free(input);
	return NO_ERROR;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state		state;
	char		*input;
	t_error		err;

	(void)argc;
	(void)argv;
	err = from_envp((const char **)envp, &state.env);
	if (err != NO_ERROR)
		return EXIT_FAILURE;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		if (!input)
			break; /* eof */
		err = run_command(input, &state);
		printf("command status: %s\n", error_repr(err));
	}
	clear_history(); // BAD, it's only here bc i couldnt make it compile with rl_x - poss
}
