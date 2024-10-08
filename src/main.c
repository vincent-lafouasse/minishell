#include <stdlib.h>
#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "error/t_error.h"
#include "log/log.h"
#include "parse/parse.h"

#define SHELL_PROMPT "minishell$ "

int	main(void)
{
	char			*input;
	t_error			err;
	t_command		cmd;

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

		//syntax_tree_to_json(cmd);
		free(input);
	}
	clear_history(); // BAD, it's only here bc i couldnt make it compile with rl_x - poss
}
