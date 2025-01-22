/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_conditional.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:04:07 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 18:04:08 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse/t_command/t_command.h"
#include <assert.h>
#include <sys/wait.h>

t_command_result	execute_conditional(t_state *state, t_conditional *cond)
{
	t_conditional_operator	op;
	t_command				first;
	t_command				second;
	int						status;
	t_command_result		first_res;

	op = cond->op;
	first = cond->first;
	second = cond->second;
	first_res = execute_command(state, first); // bad, may err
	status = first_res.status_code;
	if (status != 0 && op == AND_OP) // false &&
		return (first_res);
	if (status == 0 && op == OR_OP) //  true ||
		return (first_res);
	return (execute_command(state, second));
}
