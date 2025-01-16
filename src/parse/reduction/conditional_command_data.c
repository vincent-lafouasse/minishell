/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditional_command_data.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:54:35 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 19:54:58 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "associativity_inversal_internals.h"
#include "libft/stdlib.h"
#include <stdlib.h>

t_cond_data	cond_data_allocate(size_t connector_count)
{
	t_command				*commands;
	t_conditional_operator	*operators;

	commands = ft_calloc(connector_count + 1, sizeof(*commands));
	operators = ft_calloc(connector_count, sizeof(*operators));
	if (commands == NULL || operators == NULL)
	{
		free(commands);
		free(operators);
		return ((t_cond_data){0});
	}
	return ((t_cond_data){.commands = commands, .operators = operators,
		.connector_count = connector_count});
}

void	destroy_cond_data(t_cond_data data, size_t start)
{
	size_t	i;

	i = start;
	while (i <= data.connector_count)
	{
		command_destroy(data.commands[i]);
		i++;
	}
}
