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
#include "io/t_redir_list/t_redir_list.h"

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

	err = gather_here_documents(cmd);
	if (err != NO_ERROR)
	{
		command_destroy(cmd);
		return (err);
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

void truncate_to_one_line_if_necessary(char *input)
{
	char *line_break;

	line_break = ft_strchr(input, '\n');
	if (line_break)
		*line_break = '\0';
}

char *read_a_line(void)
{
	char *input;
	int old_last_signal; // XXX is this necessary?

	old_last_signal = last_signal;
	while (1)
	{
		last_signal = 0;
		input = readline(SHELL_PROMPT);
		if (input == NULL)
			return (NULL);
		if (last_signal != SIGINT)
			break;
		free(input); /* we've caught a C-c signal; repeat */
	}

	// TODO: double check that bash really does behaves like this
	if (*input != '\0')
		add_history(input);

	truncate_to_one_line_if_necessary(input);
	last_signal = old_last_signal;
	return (input);
}

void run_interpreter(t_state* state)
{
	char		*input;
	t_error err;

	while (1)
	{
		install_interactive_handlers();
		input = read_a_line();
		if (!input)
			break; /* eof or read error */
		//install_execution_handlers();

		err = run_and_parse_command(input, state); // bad, error should be handled
		free(input);
		printf("command status: %s\n", error_repr(err));
	}
	rl_clear_history();
	// TODO: call `exit` builtin on Ctrl-D
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
