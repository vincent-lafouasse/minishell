/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:12:16 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 21:57:47 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell/shell.h"
#include "error/t_error.h"
#include "signal/signal.h"
#include "execute/execute.h"
#include "parse/parse.h"
#include "word/word.h"

#include "libft/ft_io.h"
#include "libft/string.h"

#include <stdlib.h>
#include <signal.h>

#define USAGE "./minishell [-c command]\n"

t_error	run_and_parse_command(const char *input, t_state *state)
{
	t_error				err;
	t_command_result	res;

	err = parse(input, &state->root);
	if (err != NO_ERROR)
	{
		state->last_status = parse_error_exit_code(err);
		return (err);
	}
	if (!command_is_initialized(state->root))
		return (NO_ERROR);
	if (g_last_signal == SIGINT)
	{
		state->last_status = 128 + SIGINT;
		return (command_destroy_and_clear(&state->root), E_INTERRUPTED);
	}
	err = gather_here_documents(state->root);
	if (err != NO_ERROR)
		return (command_destroy_and_clear(&state->root), err);
	res = execute_command(state, state->root);
	command_destroy_and_clear(&state->root);
	state->last_status = res.status_code;
	return (res.error);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state	state;
	t_error	err;
	bool	executing_arg_command;

	executing_arg_command = argc == 3 && ft_strncmp(argv[1], "-c", 3) == 0;
	err = shell_init(envp, executing_arg_command, &state);
	if (err != NO_ERROR)
		return (EXIT_FAILURE);
	if (argc == 1 && state.is_interactive)
		run_interpreter(&state);
	else if (argc == 1 && !state.is_interactive)
		run_non_interactive_loop(&state);
	else if (executing_arg_command)
		run_argument_command(&state, argv[2]);
	else
		ft_putstr(USAGE);
	shell_exit(&state, state.last_status);
}
