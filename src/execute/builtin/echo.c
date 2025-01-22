/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:29:57 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:29:58 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"
#include "builtin.h"
#include "error/t_error.h"
#include "libft/ft_io.h"
#include "libft/string.h"
#include "word/t_string/t_string.h"
#include "word/t_word_list/t_word_list.h"
#include <stdlib.h>
#include <unistd.h>

#define VALID_FLAGS "n"

static bool	is_valid_argument_list(const char *sequence)
{
	size_t	i;

	i = 0;
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

static bool	advance_and_find_n_argument(t_word_list **arguments)
{
	t_word_list	*current;
	const char	*current_contents;
	bool		res;

	res = true;
	current = *arguments;
	while (current && *current->contents == '-')
	{
		current_contents = current->contents + 1;
		if (!is_valid_argument_list(current_contents))
			break ;
		res = false;
		current = current->next;
	}
	*arguments = current;
	return (res);
}

static t_error	echo_into_string(t_string **str, t_word_list *words,
		bool finish_with_newline)
{
	t_word_list	*current;

	current = words;
	while (current)
	{
		if (string_extend(str, current->contents))
			return (E_OOM);
		if (current->next)
		{
			if (string_extend(str, " "))
				return (E_OOM);
		}
		current = current->next;
	}
	if (finish_with_newline)
	{
		if (string_extend(str, "\n"))
			return (E_OOM);
	}
	return (NO_ERROR);
}

t_command_result	execute_echo(t_state *state, t_simple *builtin)
{
	bool		should_print_newline;
	t_string	*out;
	t_word_list	*words;
	t_error		err;
	int			chars_written;

	(void)state;
	words = builtin->words->next;
	should_print_newline = advance_and_find_n_argument(&words);
	out = string_new();
	if (!out)
		return ((t_command_result){.error = E_OOM});
	err = echo_into_string(&out, words, should_print_newline);
	if (err != NO_ERROR)
	{
		string_destroy(out);
		return ((t_command_result){.error = err});
	}
	chars_written = write(STDOUT_FILENO, &out->data, out->len);
	string_destroy(out);
	if (chars_written < 0)
		return ((t_command_result){.error = NO_ERROR,
			.status_code = EXIT_FAILURE});
	return ((t_command_result){.error = NO_ERROR, .status_code = 0});
}
