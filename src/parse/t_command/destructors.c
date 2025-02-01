/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:31:00 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:31:16 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
