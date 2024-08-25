#include <readline/readline.h>
#include <stdlib.h>

#include "tokenize/t_token.h"
#include "parse/parse.h"

#define SHELL_PROMPT "minishell$ "

int	main(void)
{
	char	*input;
	TokenList tokens;
	t_command command;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		tokens = tokenize(input);
		command = parse_command(tokens);
		(void)command;
		free(input);
	}
}
