/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:55 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:57:55 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/stdlib.h"
#include "t_command.h"

t_command	command_new_empty(t_command_type type)
{
	void	*alloc;

	if (type == CMD_PIPELINE)
		alloc = ft_calloc(1, sizeof(t_pipeline));
	else if (type == CMD_CONDITIONAL)
		alloc = ft_calloc(1, sizeof(t_conditional));
	else if (type == CMD_SUBSHELL)
		alloc = ft_calloc(1, sizeof(t_subshell));
	else if (type == CMD_SIMPLE)
		alloc = ft_calloc(1, sizeof(t_simple));
	else
		alloc = NULL;
	return ((t_command){.type = type, .any = alloc});
}
