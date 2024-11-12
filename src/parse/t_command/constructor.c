/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:49 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:57:52 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io/t_redir_list/t_redir_list.h"
#include "libft/stdlib.h"
#include "t_command.h"
#include "word/t_word_list/t_word_list.h"
#include <stddef.h>

t_pipeline	*pipeline_new(t_command first, t_command second)
{
	t_pipeline	*pipeline;

	pipeline = ft_calloc(1, sizeof(*pipeline));
	if (!pipeline)
		return (NULL);
	*pipeline = (t_pipeline){first, second};
	return (pipeline);
}

t_command	command_from_pipeline(t_pipeline *pipeline)
{
	return ((t_command){.type = CMD_PIPELINE, .pipeline = pipeline});
}

t_command	command_new_pipeline(t_command first, t_command second)
{
	return (command_from_pipeline(pipeline_new(first, second)));
}

t_conditional	*conditional_new(t_conditional_operator op, t_command first,
		t_command second)
{
	t_conditional	*conditional;

	conditional = ft_calloc(1, sizeof(*conditional));
	if (!conditional)
		return (NULL);
	*conditional = (t_conditional){op, first, second};
	return (conditional);
}

t_command	command_from_conditional(t_conditional *conditional)
{
	return ((t_command){.type = CMD_CONDITIONAL, .conditional = conditional});
}

t_command	command_new_conditional(t_conditional_operator op, t_command first,
		t_command second)
{
	return (command_from_conditional(conditional_new(op, first, second)));
}

t_subshell	*subshell_new(t_command cmd, t_redir_list *redirections)
{
	t_subshell	*subshell;

	subshell = ft_calloc(1, sizeof(*subshell));
	if (!subshell)
		return (NULL);
	*subshell = (t_subshell){.cmd = cmd, .redirections = redirections};
	return (subshell);
}

t_command	command_from_subshell(t_subshell *subshell)
{
	return ((t_command){.type = CMD_SUBSHELL, .subshell = subshell});
}

t_command	command_new_subshell(t_command cmd, t_redir_list *redirections)
{
	return (command_from_subshell(subshell_new(cmd, redirections)));
}

t_simple	*simple_new(t_word_list *words, t_redir_list *redirections)
{
	t_simple	*simple;

	simple = ft_calloc(1, sizeof(*simple));
	if (!simple)
		return (NULL);
	*simple = (t_simple){.words = words, .redirections = redirections};
	return (simple);
}

t_command	command_from_simple(t_simple *simple)
{
	return ((t_command){.type = CMD_SIMPLE, .simple = simple});
}

t_command	command_new_simple(t_word_list *words, t_redir_list *redirections)
{
	return (command_from_simple(simple_new(words, redirections)));
}
