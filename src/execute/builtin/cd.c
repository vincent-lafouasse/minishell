#include "builtin.h"

#include "../execute.h"
#include "word/t_word_list/t_word_list.h"
#include "libft/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USAGE_MESSAGE "usage: cd relative_or_absolute_path\n"

t_command_result execute_cd(t_state *state, t_simple *builtin)
{
	(void)state;

	t_word_list *arguments;
	arguments = builtin->words->next;

	if (!arguments || arguments->next != NULL)
	{
		write(STDERR_FILENO, USAGE_MESSAGE, ft_strlen(USAGE_MESSAGE));
		return command_ok(EX_BADUSAGE);
	}

	if (chdir(arguments->contents) < 0)
	{
		perror("minishell: cd");
		return command_ok(EXIT_FAILURE);
	}

	return command_ok(EXIT_SUCCESS);
}
