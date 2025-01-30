/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_destroy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:53 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:31:39 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection/t_redir_list/t_redir_list.h"
#include "t_command.h"

void	pipeline_destroy(t_pipeline *pipeline);
void	conditional_destroy(t_conditional *conditional);
void	subshell_destroy(t_subshell *subshell);
void	simple_destroy(t_simple *simple);

void	command_destroy_and_clear(t_command *command)
{
	command_destroy(*command);
	*command = (t_command){0};
}

void	command_destroy(t_command command)
{
	if (!command_is_initialized(command))
		return ;
	if (command.type == CMD_SIMPLE)
		return (simple_destroy(command.simple));
	else if (command.type == CMD_SUBSHELL)
		return (subshell_destroy(command.subshell));
	else if (command.type == CMD_PIPELINE)
		return (pipeline_destroy(command.pipeline));
	else if (command.type == CMD_CONDITIONAL)
		return (conditional_destroy(command.conditional));
}
