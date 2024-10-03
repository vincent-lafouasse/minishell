#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "error/t_error.h"
#include "log/log.h"
#include "parse/parse.h"
#include "tokenize/tokenize.h"

#include "parse/reduction/reduction.h"
#include "parse/reduction/reduction_internals.h"

#define SHELL_PROMPT "minishell$ "

// TODO: copy bash implementation
int	line_should_be_saved_in_history(const char *input)
{
	return (*input != '\0');
}

int	main(void)
{
	char			*input;
	t_token_list	*tokens;
	t_symbol		parse_tree;
	t_error			err;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		if (line_should_be_saved_in_history(input))
			add_history(input);
		tokens = tokenize(input);
		if (!tokens)
			continue;

		err = parse_command(tokens, &parse_tree);
		if (err != NO_ERROR)
		{
			printf("symbol status: %s\n", error_repr(err));
			continue;
		}

		t_command cmd = reduce_parse_tree_into_command(&parse_tree);

		syntax_tree_to_json(cmd);
		free(input);
		tkl_clear(&tokens);
	}
	clear_history();
}
