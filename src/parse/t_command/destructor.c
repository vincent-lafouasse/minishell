/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:53 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:57:53 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io/t_redir_list/t_redir_list.h"
#include "t_command.h"
#include "word/t_word_list/t_word_list.h"
#include <stdlib.h>

void	pipeline_destroy(t_pipeline *pipeline)
{
	if (pipeline == NULL)
		return ;
	command_destroy(pipeline->first);
	command_destroy(pipeline->second);
	free(pipeline);
}

void	conditional_destroy(t_conditional *conditional)
{
	if (conditional == NULL)
		return ;
	command_destroy(conditional->first);
	command_destroy(conditional->second);
	free(conditional);
}

void	subshell_destroy(t_subshell *subshell)
{
	if (subshell == NULL)
		return ;
	command_destroy(subshell->cmd);
	rdl_clear(&subshell->redirections, free);
	free(subshell);
}

void	simple_destroy(t_simple *simple)
{
	if (simple == NULL)
		return ;
	rdl_clear(&simple->redirections, free);
	wl_clear(&simple->words, free);
	free(simple);
}

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
