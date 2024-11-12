/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:39 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:25:40 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <assert.h> // temporarily

// command : simple_command
//         | subshell redirect_list
//         | subshell
t_error	reduce_command(t_symbol *root, t_command *out)
{
	t_error err;

	assert (root->kind == SYM_COMMAND);
	if (root->production->data[0].kind == SYM_SIMPLE_CMD)
		err = reduce_simple_command(&root->production->data[0], out);
	else
		err = reduce_subshell(&root->production->data[0], \
							   &root->production->data[1], out);
	return err;
}
