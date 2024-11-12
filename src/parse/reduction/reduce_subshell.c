/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:50 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:25:50 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <assert.h> // temporarily

t_error	reduce_subshell(t_symbol *subshell, t_symbol *trailing_redirs, t_command *out)
{
	t_error		err;

	assert (subshell->kind == SYM_SUBSHELL);
	assert (trailing_redirs->kind == SYM_SUBSHELL_PRECEDES);

	out->subshell = subshell_new((t_command){0}, NULL);
	if (!out->subshell)
		return (E_OOM);

	err = reduce_complete_command(&subshell->production->data[1], &out->subshell->cmd);
	if (err != NO_ERROR)
		return (err);
	err = reduce_simple_command_like(trailing_redirs, NULL, &out->subshell->redirections);
	if (err != NO_ERROR)
		return (err);

	*out = command_from_subshell(out->subshell);
	return (NO_ERROR);
}
