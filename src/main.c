#include <readline/readline.h>
#include <stdlib.h>

#include "parse/parse.h"
#include "tokenize/tokenize.h"

#define SHELL_PROMPT "minishell$ "

int	main(void)
{
	char			*input;
	t_token_list	*tokens;
	t_command		command;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		tokens = tokenize(input);
		command = parse_command(tokens);
		(void)command;
		free(input);
	}
}
