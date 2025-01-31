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

#include "error/t_error.h"
#include "execute/execute.h"
#include "execute/t_env/t_env.h"
#include "libft/ft_io.h"
#include "libft/string.h"
#include "parse/parse.h"
#include "parse/t_command/t_command.h"
#include "redirection/t_io/t_io.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include "shell/shell.h"
#include "signal/signal.h"
#include <assert.h> // temporarily, run_command
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <word/word.h>

#define SHELL_PROMPT "minishell$ "
#define USAGE "./minishell [-c command]"

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
		command_destroy_and_clear(&state->root);
		return (E_INTERRUPTED);
	}
	err = gather_here_documents(state->root);
	if (err != NO_ERROR)
	{
		command_destroy_and_clear(&state->root);
		return (err);
	}
	res = execute_command(state, state->root);
	command_destroy_and_clear(&state->root);
	state->last_status = res.status_code;
	return (res.error);
}

void	truncate_to_one_line_if_necessary(char *input)
{
	char	*line_break ;

	line_break = ft_strchr(input, '\n');
	if (line_break)
		*line_break = '\0';
}

char	*interactive_read_line(t_state *state)
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

char	*non_interactive_read_line(t_state *state)
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
	{
		state.is_interactive = false;
		truncate_to_one_line_if_necessary(argv[2]);
		err = run_and_parse_command(argv[2], &state);
		if (err != NO_ERROR)
			report_error_message(error_repr(err));
	}
	else
		printf("%s\n", USAGE);
	shell_exit(&state, state.last_status);
}
