#include "../builtin.h"

#include "execute/execute.h"
#include "error/t_error.h"
#include "word/t_word_list/t_word_list.h"
#include "libft/string.h"
#include "libft/ctype.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

bool    checked_atoi(const char *s, int32_t *out);

void shell_cleanup(t_state *state); // bad: should be #include "shell.h"

_Noreturn
static void exit_and_cleanup(t_state *state, int32_t exit_status)
{
	shell_cleanup(state);
	exit(exit_status & 255); // bad, doesn't clean up before exiting
}

t_command_result execute_exit(t_state *state, t_simple *builtin)
{
	t_word_list *args;
	int32_t exit_status;

	if (state->is_interactive)
		write(STDERR_FILENO, "exit\n", 5);

	args = builtin->words->next;
	if (!args)
		exit_and_cleanup(state, state->last_status);
	if (args->next != NULL)
	{
		const char *error = "minishell: exit: too many arguments\n";
		write(STDERR_FILENO, error, ft_strlen(error));
		return (t_command_result){.error = NO_ERROR, .status_code = EX_BADUSAGE};
	}
	if (!checked_atoi(args->contents, &exit_status))
	{
		const char *error = "minishell: exit: numeric argument required\n";
		write(STDERR_FILENO, error, ft_strlen(error));
		exit_and_cleanup(state, EX_BADUSAGE);
	}
	exit_and_cleanup(state, exit_status);
}
