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

int	main(int argc, char *argv[], char *envp[])
{
	char		*input;
	t_error		err;
	t_command	cmd;
	t_state		state;

	(void)argc;
	(void)argv;
	err = from_envp((const char **)envp, &state.env);
	if (err != NO_ERROR)
		return EXIT_FAILURE;
	//log_env(state.env);
	char **path = env_make_path_or_empty(state.env);
	for (size_t i = 0; path[i] != NULL; i++)
	{
		printf("%s\n", path[i]);
		free(path[i]);
	}
	free(path);
	while (1)
	{
		input = readline(SHELL_PROMPT);
		if (!input)
			break; /* eof */
		// TODO: double check that bash really does behaves like this
		if (*input == '\0')
		{
			free(input);
			continue;
		}
		add_history(input);

		err = parse(input, &cmd);
		if (err != NO_ERROR)
		{
			printf("symbol status: %s\n", error_repr(err));
			free(input);
			continue;
		}

		if (cmd.type == SIMPLE_CMD)
		{
			t_command_result res;
			res = execute_simple_command(&state, cmd.simple, (t_io){0, 1});
			log_error(res.error);

			wait(NULL);
		}

		//syntax_tree_to_json(cmd);
		free(input);
	}
	clear_history(); // BAD, it's only here bc i couldnt make it compile with rl_x - poss
}
