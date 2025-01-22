/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:00:19 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:00:20 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin.h"
#include "execute/execute.h"
#include "export_internals.h"

t_command_result	execute_export(t_state *state, t_simple *builtin)
{
	t_word_list	*assignments;

	assignments = builtin->words->next;
	if (assignments)
		return (do_assignments(&state->env, assignments));
	else
		return (print_all_variables(state));
}
