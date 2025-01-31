/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:49:44 by jamar             #+#    #+#             */
/*   Updated: 2025/01/31 22:49:45 by jamar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell/shell.h"
#include "error/t_error.h"
#include "execute/t_env/t_env.h"

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

static t_error	set_up_environment(t_env **env, char *envp[]);
static t_error	add_if_not_set(t_env **env, const char *key, const char *value);
static void		init_interactive(t_state *state_out);

t_error	shell_init(char *envp[], bool running_arg_command, t_state *state_out)
{
	t_error	err;

	rl_outstream = stderr;
	*state_out = (t_state){0};
	err = set_up_environment(&state_out->env, envp);
	if (err != NO_ERROR)
	{
		report_t_error("set_up_environment", err);
		return (err);
	}
	if (!running_arg_command && isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		init_interactive(state_out);
	else
		state_out->is_interactive = false;
	return (NO_ERROR);
}

static t_error	set_up_environment(t_env **env, char *envp[])
{
	t_error	err;

	err = from_envp(envp, env);
	if (err != NO_ERROR)
		return (err);
	err = add_if_not_set(env, "TERM", "dumb");
	if (err != NO_ERROR)
	{
		env_destroy(env);
		return (NO_ERROR);
	}
	err = add_if_not_set(env, "PATH", DEFAULT_PATH_VALUE);
	if (err != NO_ERROR)
	{
		env_destroy(env);
		return (NO_ERROR);
	}
	return (NO_ERROR);
}

static t_error	add_if_not_set(t_env **env, const char *key, const char *value)
{
	if (env_key_exists(*env, key))
		return (NO_ERROR);
	return (env_insert(env, key, value));
}

static void	init_interactive(t_state *state_out)
{
	int	ret;

	ret = tcgetattr(STDERR_FILENO, &state_out->tty_properties);
	state_out->tty_properties_initialized = ret == 0;
	state_out->is_interactive = true;
}
