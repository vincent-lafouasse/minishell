#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "error/t_error.h"
#include "parse/parse.h"
#include "tokenize/tokenize.h"

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
	t_symbol		symbol;
	t_error			err;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		if (line_should_be_saved_in_history(input))
			add_history(input);
		tokens = tokenize(input);
		if (!tokens)
			continue;
		err = parse_command(tokens, &symbol);
		printf("symbol status: %s\n", error_repr(err));
		free(input);
		tkl_clear(&tokens);
	}
	rl_clear_history();
}
