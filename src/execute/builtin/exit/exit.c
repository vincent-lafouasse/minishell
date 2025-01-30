/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:18:58 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:18:58 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin.h"
#include "execute/execute.h"
#include "libft/string.h"
#include "shell/shell.h"
#include "word/t_word_list/t_word_list.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define EXIT_NON_NUMERIC "minishell: exit: numeric argument required\n"
#define EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments\n"

bool	checked_atoi(const char *s, int32_t *out);

t_command_result	execute_exit(t_state *state, t_simple *builtin)
{
	t_word_list	*args;
	int32_t		exit_status;

	if (state->is_interactive)
		write(STDERR_FILENO, "exit\n", 5);
	args = builtin->words->next;
	if (!args)
		shell_exit(state, state->last_status);
	if (!checked_atoi(args->contents, &exit_status))
	{
		write(STDERR_FILENO, EXIT_NON_NUMERIC, ft_strlen(EXIT_NON_NUMERIC));
		shell_exit(state, EX_BADUSAGE);
	}
	if (args->next != NULL)
	{
		write(STDERR_FILENO, EXIT_TOO_MANY_ARGS, ft_strlen(EXIT_TOO_MANY_ARGS));
		return (command_ok(EX_BADUSAGE));
	}
	shell_exit(state, exit_status);
}
