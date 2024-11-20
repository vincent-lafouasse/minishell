/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_constructor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:49 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:07:29 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io/t_redir_list/t_redir_list.h"
#include "libft/stdlib.h"
#include "t_command.h"
#include "word/t_word_list/t_word_list.h"
#include <stddef.h>

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
