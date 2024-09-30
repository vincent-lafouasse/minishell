#include <readline/readline.h>
#include <stdlib.h>

#include "error/t_error.h"
#include "parse/parse.h"
#include "tokenize/tokenize.h"

#define SHELL_PROMPT "minishell$ "

int	main(void)
{
	char			*input;
	t_token_list	*tokens;
	t_symbol		symbol;
	t_error			err;

	while (1)
	{
		input = readline(SHELL_PROMPT);
		tokens = tokenize(input);
		if (!tokens)
			continue;
		err = parse_command(tokens, &symbol);
		printf("symbol status: %s\n", error_repr(err));
		free(input);
		tkl_clear(&tokens);
	}
}
