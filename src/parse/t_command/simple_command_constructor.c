/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command_constructor.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:49 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:07:16 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io/t_redir_list/t_redir_list.h"
#include "libft/stdlib.h"
#include "t_command.h"
#include "word/t_word_list/t_word_list.h"
#include <stddef.h>

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
