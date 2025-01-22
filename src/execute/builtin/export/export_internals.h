/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_internals.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:51:57 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 16:58:14 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_INTERNALS_H
# define EXPORT_INTERNALS_H

# include <stddef.h>
#include "execute/execute.h"

// vvv do_assignments.c
t_command_result do_assignments(t_env **env, t_word_list *assignments);

// vvv print_all_variables.c
t_command_result print_all_variables(t_state *state);


// vvv export_internals.c
// returns 0 if assignment is malformated in any way
size_t	name_len(char *assignment);
void	report_invalid_identifier(char *identifier_name);

#endif
