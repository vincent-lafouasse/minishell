/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:06:08 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:07:07 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "execute/t_env/t_env.h"
# include "execute/t_pid_list/t_pid_list.h"
# include "parse/t_command/t_command.h"
# include <stdbool.h>
# include <termios.h>

typedef struct s_state
{
	t_env			*env;
	t_command		root;
	int				last_status;
	bool			is_interactive;
	char			*line;
	bool			tty_properties_initialized;
	struct termios	tty_properties;
	t_pid_list		*our_children;
}					t_state;

void	shell_cleanup(t_state *state);
void	shell_exit(t_state *state, int exit_status) __attribute__((noreturn));

#endif // SHELL_H
