/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_constructor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:49 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:06:55 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/stdlib.h"
#include "t_command.h"
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
