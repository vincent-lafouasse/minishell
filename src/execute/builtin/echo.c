#include "builtin.h"

#include "../execute.h"
#include "word/t_string/t_string.h"
#include "libft/string.h"
#include "libft/ft_io.h"
#include "word/t_word_list/t_word_list.h"

#include <unistd.h>

#define VALID_FLAGS "n"

static bool is_valid_argument_list(const char *sequence)
{
	size_t i = 0;

	if (!sequence[i])
		return (false);
	while (sequence[i])
	{
		if (sequence[i] != *VALID_FLAGS)
			return (false);
		i++;
	}
	return (true);
}

static bool advance_and_find_n_argument(t_word_list **arguments)
{
	t_word_list *current;
	const char *current_contents;
	bool res;

	res = true;
	current = *arguments;
	while (current && *current->contents == '-')
	{
		current_contents = current->contents + 1;
		if (!is_valid_argument_list(current_contents))
			break;
		res = false;
		current = current->next;
	}
	*arguments = current;
	return (res);
}

t_command_result execute_echo(t_state *state, t_simple *builtin)
{
	(void)state;

	bool should_print_newline;
	t_string *out;
	t_word_list* words = builtin->words->next;

	should_print_newline = advance_and_find_n_argument(&words);

	out = string_new();
	if (!out)
		return (t_command_result){.error = E_OOM};

	while (words)
	{
		if (string_extend(&out, words->contents)) {
			string_destroy(out);
			return (t_command_result){.error = E_OOM};
		}
		if (words->next)
		{
			if (string_extend(&out, " ")) {
				string_destroy(out);
				return (t_command_result){.error = E_OOM};
			}
		}
		words = words->next;
	}
	if (should_print_newline)
	{
		if (string_extend(&out, "\n")) {
			string_destroy(out);
			return (t_command_result){.error = E_OOM};
		}
	}

	write(STDOUT_FILENO, &out->data, out->len); // bad check for error, and potentially return status code `EXIT_FAILURE`
	return (t_command_result){.error = NO_ERROR, .status_code = 0};
}
