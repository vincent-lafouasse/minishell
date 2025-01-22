/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:45:33 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 16:47:38 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin.h"
#include "error/t_error.h"
#include "execute/execute.h"
#include "libft/ctype.h"
#include "libft/string.h"
#include "word/t_word_list/t_word_list.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMERIC_ARGUMENT_REQUIRED "minishell: exit: numeric argument required\n"
#define TOO_MANY_ARGUMENTS "minishell: exit: too many arguments\n"

bool				checked_atoi(const char *s, int32_t *out);

// bad, `exit.c:exit_and_cleanup` doesn't clean up before exiting
_Noreturn static void	exit_and_cleanup(t_state *state, int32_t exit_status)
{
	(void)state;
	exit(exit_status & 255);
}

t_command_result	execute_exit(t_state *state, t_simple *builtin)
{
	t_word_list	*args;
	int32_t		exit_status;

	if (state->is_interactive)
		write(STDERR_FILENO, "exit\n", 5);
	args = builtin->words->next;
	if (!args)
		exit_and_cleanup(state, state->last_status);
	if (!checked_atoi(args->contents, &exit_status))
	{
		write(STDERR_FILENO, NUMERIC_ARGUMENT_REQUIRED,
			ft_strlen(NUMERIC_ARGUMENT_REQUIRED));
		exit_and_cleanup(state, EX_BADUSAGE);
	}
	if (args->next != NULL)
	{
		write(STDERR_FILENO, TOO_MANY_ARGUMENTS, ft_strlen(TOO_MANY_ARGUMENTS));
		return ((t_command_result){.error = NO_ERROR,
			.status_code = EX_BADUSAGE});
	}
	exit_and_cleanup(state, exit_status);
}
