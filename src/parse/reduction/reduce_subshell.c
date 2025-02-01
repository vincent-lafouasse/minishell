/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:50 by poss              #+#    #+#             */
/*   Updated: 2025/02/01 22:02:17 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"
#include "reduction_internals.h"

t_error	reduce_subshell(t_symbol *subshell, t_symbol *trailing_redirs,
		t_command *out)
{
	t_error			err;
	t_symbol_array	*productions;

	productions = subshell->production;
	*out = command_new_subshell((t_command){0}, NULL);
	if (!command_is_initialized(*out))
		return (E_OOM);
	err = reduce_complete_command(&productions->data[1], &out->subshell->cmd);
	if (err != NO_ERROR)
		return (err);
	err = reduce_simple_command_like(trailing_redirs, NULL,
			&out->subshell->redirections);
	if (err != NO_ERROR)
		return (err);
	return (NO_ERROR);
}
