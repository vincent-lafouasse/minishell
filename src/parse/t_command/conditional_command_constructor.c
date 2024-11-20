/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditional_command_constructor.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:49 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:06:32 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/stdlib.h"
#include "t_command.h"
#include <stddef.h>

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
