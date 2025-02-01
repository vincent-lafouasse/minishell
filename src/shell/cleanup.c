/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:06:00 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:07:36 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/t_command/t_command.h"
#include "shell.h"
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

__attribute__((noreturn)) void	shell_exit(t_state *state, int exit_status)
{
	shell_cleanup(state);
	exit(exit_status);
}

void	shell_cleanup(t_state *state)
{
	command_destroy_and_clear(&state->root);
	free(state->line);
	rl_clear_history();
	env_destroy(&state->env);
	pidl_clear(&state->our_children);
}
