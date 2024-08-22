#include <readline/readline.h>
#include <stdlib.h>

#define SHELL_PROMPT "minishell$ "

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		free(input);
	}
}
