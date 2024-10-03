#include <stdlib.h>
#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "error/t_error.h"
#include "log/log.h"
#include "parse/parse.h"

#define SHELL_PROMPT "minishell$ "

// TODO: copy bash implementation
int	line_should_be_saved_in_history(const char *input)
{
	return (*input != '\0');
}

int	main(void)
{
	char			*input;
	t_error			err;
	t_command			cmd;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		if (line_should_be_saved_in_history(input))
			add_history(input);

		err = parse(input, &cmd);
		if (err != NO_ERROR)
		{
			printf("symbol status: %s\n", error_repr(err));
			free(input);
			continue;
		}

		syntax_tree_to_json(cmd);
		free(input);
	}
	clear_history();
}
