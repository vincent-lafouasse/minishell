#include "builtin.h"

#include "../execute.h"
#include "error/t_error.h"
#include "word/t_word_list/t_word_list.h"
#include "libft/string.h"
#include "libft/ctype.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

_Noreturn
static void exit_and_cleanup(t_state *state, long long exit_status)
{
	(void)state;
	exit(exit_status & 255); // bad, doesn't clean up before exiting
}

static bool number_fits_in_long_long(const char *status) // bad, dummy impl
{
	(void)status;
	return (true);
}

// NOTE(bit): i get a linker error locally every time i try to compile with libft
//
// Undefined symbols for architecture arm64:
//   "_ft_isspace", referenced from:
//       _is_valid_number in exit.c.o
// ld: symbol(s) not found for architecture arm64
static bool hack_isspace(char c) // bad, replace this with the libft implementation
{
	return (c == ' ');
}

// TODO: replicate bash number validation
static bool is_valid_number(const char *num)
{
	while (hack_isspace(*num)) num++;
	if (*num == '-' || *num == '+') num++;
	if (!*num)
		return (false);
	while (ft_isdigit(*num)) num++;
	return (*num == '\0');
}

static bool parse_status_code(const char *str, long long *out)
{
	if (!is_valid_number(str) || !number_fits_in_long_long(str))
		return (false);
	*out = atoi(str); // bad, should be atoll
	return (true);
}

t_command_result execute_exit(t_state *state, t_simple *builtin)
{
	t_word_list *args;
	long long exit_status;

	if (state->is_interactive)
		write(STDERR_FILENO, "exit\n", 5);

	args = builtin->words->next;
	if (!args)
		exit_and_cleanup(state, state->last_status);
	if (args->next != NULL)
	{
		const char *error = "minishell: exit: too many arguments\n";
		write(STDERR_FILENO, error, ft_strlen(error));
		return (t_command_result){.error = NO_ERROR, .status_code = EXIT_FAILURE};
	}
	if (!parse_status_code(args->contents, &exit_status))
	{
		const char *error = "minishell: exit: numeric argument required\n";
		write(STDERR_FILENO, error, ft_strlen(error));
		exit_and_cleanup(state, EX_BUILTIN_BAD_USAGE);
	}
	exit_and_cleanup(state, exit_status);
}
