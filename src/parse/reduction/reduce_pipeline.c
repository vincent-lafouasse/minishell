/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:45 by poss              #+#    #+#             */
/*   Updated: 2025/02/01 22:03:44 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"
#include "reduction_internals.h"
#include <stdbool.h>

static t_error	reduce_pipeline_rest(t_symbol *pipeline_rest, t_command *out)
{
	t_symbol_array	*productions;
	t_error			err;

	productions = pipeline_rest->production;
	if (productions->data[2].production->len == 0)
		return (reduce_command(&productions->data[1], out));
	*out = command_new_pipeline((t_command){0}, (t_command){0});
	if (!command_is_initialized(*out))
		return (E_OOM);
	err = reduce_command(&productions->data[1], &out->pipeline->first);
	if (err != NO_ERROR)
		return (err);
	err = reduce_pipeline_rest(&productions->data[2], &out->pipeline->second);
	return (err);
}

t_error	reduce_pipeline(t_symbol *pipeline, t_command *out)
{
	t_error			err;
	t_symbol_array	*productions;

	productions = pipeline->production;
	if (productions->data[1].production->len == 0)
		return (reduce_command(&productions->data[0], out));
	*out = command_new_pipeline((t_command){0}, (t_command){0});
	if (!command_is_initialized(*out))
		return (E_OOM);
	err = reduce_command(&productions->data[0], &out->pipeline->first);
	if (err != NO_ERROR)
		return (err);
	err = reduce_pipeline_rest(&productions->data[1], &out->pipeline->second);
	if (err != NO_ERROR)
		return (err);
	return (NO_ERROR);
}
