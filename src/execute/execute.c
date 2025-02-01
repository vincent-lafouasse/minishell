/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:46:34 by poss              #+#    #+#             */
/*   Updated: 2025/02/01 21:59:34 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./builtin/builtin.h"
#include "execute.h"

t_command_result	execute_command(t_state *state, t_command command)
{
	t_command_result	res;

	if (command.type == CMD_SIMPLE)
		res = execute_simple_or_builtin(state, command.simple);
	else if (command.type == CMD_PIPELINE)
		res = execute_pipeline(state, command.pipeline);
	else if (command.type == CMD_CONDITIONAL)
		res = execute_conditional(state, command.conditional);
	else if (command.type == CMD_SUBSHELL)
		res = execute_subshell(state, command.subshell);
	else
		res = command_err(E_UNREACHABLE);
	return (res);
}
