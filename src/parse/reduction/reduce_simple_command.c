/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce_simple_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:47 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:25:47 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <assert.h> // temporarily

t_error	reduce_simple_command(t_symbol *root, t_command *out)
{
	t_error		err;

	assert (root->kind == SYM_SIMPLE_CMD);

	out->simple = simple_new(NULL, NULL);
	if (!out->simple)
		return (E_OOM);

	err = reduce_simple_command_like(root, &out->simple->words, &out->simple->redirections);
	if (err != NO_ERROR)
		return (err);

	*out = command_from_simple(out->simple);
	return (NO_ERROR);
}
