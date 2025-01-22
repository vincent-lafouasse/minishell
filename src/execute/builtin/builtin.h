/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:29:30 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:29:52 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../execute.h"
# include "parse/t_command/t_command.h"
# include <stdbool.h>

# define EX_BADUSAGE 2

bool				is_builtin_command(const t_simple *simple);

t_command_result	execute_echo(t_state *state, t_simple *builtin);
t_command_result	execute_exit(t_state *state, t_simple *builtin);
t_command_result	execute_env(t_state *state, t_simple *builtin);
t_command_result	execute_pwd(t_state *state, t_simple *builtin);
t_command_result	execute_cd(t_state *state, t_simple *builtin);
t_command_result	execute_unset(t_state *state, t_simple *builtin);
t_command_result	execute_export(t_state *state, t_simple *builtin);

#endif
