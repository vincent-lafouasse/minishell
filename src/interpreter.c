/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 23:03:46 by jamar             #+#    #+#             */
/*   Updated: 2025/01/31 23:03:47 by jamar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell/shell.h"
#include "word/word.h"
#include "signal/signal.h"
#include "error/t_error.h"

#include "libft/ft_io.h"

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

static char	*interactive_read_line(t_state *state);
static char	*non_interactive_read_line(t_state *state);

void	run_interpreter(t_state *state)
{
	t_error	err;

	err = NO_ERROR;
	while (err != E_OOM)
	{
		install_interactive_handlers();
		state->line = interactive_read_line(state);
		if (!state->line)
			break ;
		err = run_and_parse_command(state->line, state);
		if (err != NO_ERROR)
			report_error_message(error_repr(err));
		free(state->line);
		state->line = NULL;
	}
	rl_clear_history();
	if (err != E_OOM)
		ft_putstr_fd("exit\n", STDERR_FILENO);
}

static char	*interactive_read_line(t_state *state)
{
	char	*input;

	while (1)
	{
		g_last_signal = 0;
		input = readline(SHELL_PROMPT);
		if (input == NULL)
			return (NULL);
		if (g_last_signal != SIGINT)
			break ;
		free(input);
		state->last_status = 128 + g_last_signal;
	}
	if (*input != '\0')
		add_history(input);
	truncate_to_one_line_if_necessary(input);
	return (input);
}

void	run_non_interactive_loop(t_state *state)
{
	t_error	err;

	err = NO_ERROR;
	while (err != E_OOM && err != E_INTERRUPTED)
	{
		install_non_interactive_handlers();
		state->line = non_interactive_read_line(state);
		if (!state->line)
			break ;
		err = run_and_parse_command(state->line, state);
		if (err != NO_ERROR)
			report_error_message(error_repr(err));
		free(state->line);
		state->line = NULL;
	}
}

static char	*non_interactive_read_line(t_state *state)
{
	char	*input;

	input = readline(NULL);
	if (g_last_signal == SIGINT || input == NULL)
	{
		if (g_last_signal == SIGINT)
			state->last_status = 128 + SIGINT;
		free(input);
		return (NULL);
	}
	truncate_to_one_line_if_necessary(input);
	return (input);
}

void	run_argument_command(t_state *state, char *command)
{
	t_error	err;

	state->is_interactive = false;
	truncate_to_one_line_if_necessary(command);
	err = run_and_parse_command(command, state);
	if (err != NO_ERROR)
		report_error_message(error_repr(err));
}
