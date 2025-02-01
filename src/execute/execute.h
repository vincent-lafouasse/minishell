/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:43:59 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:45:55 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "error/t_error.h"
# include "parse/t_command/t_command.h"
# include "redirection/t_io/t_io.h"
# include "shell/shell.h"

# define CLOSE_NOTHING -1

typedef struct s_command_result
{
	t_error						error;
	int							status_code;
}								t_command_result;

static inline t_command_result	command_ok(int status_code)
{
	return ((t_command_result){.error = NO_ERROR, .status_code = status_code});
}

static inline t_command_result	command_err(t_error err)
{
	return ((t_command_result){.error = err, .status_code = EXIT_FAILURE});
}

// internals

t_error							launch_pipeline(t_state *state,
									t_pipeline *pipeline, t_io io);
t_error							launch_simple_command(t_state *state,
									t_simple *simple, t_io io, int fd_to_close);
t_error							launch_cmd_in_subshell(t_state *state,
									t_command cmd, t_io io, int fd_to_close);
t_error							launch_subshell(t_state *state,
									t_subshell *subshell, t_io io,
									int fd_to_close);

// public

t_command_result				execute_pipeline(t_state *state,
									t_pipeline *pipeline);
t_command_result				execute_command(t_state *state,
									t_command command);
t_command_result				execute_conditional(t_state *state,
									t_conditional *cond);
t_command_result				execute_subshell(t_state *state,
									t_subshell *subshell);
t_command_result				execute_simple(t_state *state,
									t_simple *simple);
t_command_result				execute_simple_or_builtin(t_state *state,
									t_simple *simple);

t_command_result				execute_builtin(t_state *state,
									t_simple *simple);

#endif // EXECUTE_H
