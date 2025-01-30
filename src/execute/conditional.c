/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditional.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:49:00 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:49:01 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse/t_command/t_command.h"
#include <sys/wait.h>

t_command_result	execute_conditional(t_state *state, t_conditional *cond)
{
	t_command_result	first_res;
	int					status;

	first_res = execute_command(state, cond->first);
	if (first_res.error != NO_ERROR)
		return (first_res);
	status = first_res.status_code;
	if (status != 0 && cond->op == AND_OP)
		return (first_res);
	if (status == 0 && cond->op == OR_OP)
		return (first_res);
	return (execute_command(state, cond->second));
}
