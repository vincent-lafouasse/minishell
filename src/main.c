#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <assert.h> // temporarily, run_command

#include <readline/readline.h>
#include <readline/history.h>

#include "error/t_error.h"
#include "execute/execute.h"
#include "execute/t_env/t_env.h"
#include "io/t_io/t_io.h"
#include "log/log.h" // todo remove in production mode
#include "parse/parse.h"
#include "libft/string.h"
#include "parse/t_command/t_command.h"
#include "signal/signal.h"

#define SHELL_PROMPT "minishell$ "
#define USAGE "./minishell [-c command]"

t_error run_and_parse_command(const char* input, t_state* state)
{
	t_error err;
	t_command cmd;

	err = parse(input, &cmd);
	if (err != NO_ERROR)
	{
		return err;
	}

	t_command_result res = execute_command(state, cmd); // bad, should probablue check err value maybe
	/*
	switch (res.error) {
		// status_code = whatever
	}
	*/
	state->last_status = res.status_code;
	return res.error;
}

void run_interpreter(t_state* state)
{
	char		*input;
	t_error err;

	while (1)
	{
		install_interactive_handlers();
		input = readline(SHELL_PROMPT);
		if (!input)
			break; /* eof */
		//install_execution_handlers();

		// TODO: double check that bash really does behaves like this
		if (*input != '\0')
			add_history(input);

		err = run_and_parse_command(input, state);
		free(input);
		printf("command status: %s\n", error_repr(err));
	}
	rl_clear_history();
}

int	main(int argc, char *argv[], char *envp[]) // bad main should return last status
{
	t_state		state;
	t_error		err;

	err = from_envp((const char **)envp, &state.env);
	if (err != NO_ERROR)
		return EXIT_FAILURE;
	
	if (argc == 1)
		run_interpreter(&state);
	else if (argc == 3 && ft_strncmp(argv[1], "-c", 3) == 0)
		run_and_parse_command(argv[2], &state);
	else
		printf("%s\n", USAGE);
}
