#include <readline/readline.h>
#include <stdlib.h>

#include "tokenize/t_token.h"

#define SHELL_PROMPT "minishell$ "

int	main(void)
{
	char	*input;
	TokenList tokens;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		tokens = tokenize(input);
		free(input);
	}
}
