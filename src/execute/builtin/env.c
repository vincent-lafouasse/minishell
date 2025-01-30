/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:22:47 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:23:21 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"
#include "builtin.h"
#include "error/t_error.h"
#include "libft/string.h"
#include "word/t_string/t_string.h"
#include "word/t_word_list/t_word_list.h"
#include <stdlib.h>
#include <unistd.h>

#define TOO_MANY_ARGS "minishell: env: too many arguments\n"

static t_error	gather_environment(t_env *env, t_string **out)
{
	t_env_entry	curr;

	while (env)
	{
		curr = env->entry;
		if (!curr.value)
		{
			env = env->next;
			continue ;
		}
		if (string_extend(out, curr.key))
			return (E_OOM);
		if (string_push(out, '='))
			return (E_OOM);
		if (string_extend(out, curr.value))
			return (E_OOM);
		if (string_push(out, '\n'))
			return (E_OOM);
		env = env->next;
	}
	return (NO_ERROR);
}

static t_command_result	write_env_to_stdout(t_string *env)
{
	int	status;
	int	exit_code;

	status = write(STDOUT_FILENO, &env->data, env->len);
	string_destroy(env);
	if (status < 0)
	{
		exit_code = EXIT_FAILURE;
		report_error("write_env_to_stdout", "write");
	}
	else
		exit_code = EXIT_SUCCESS;
	return (command_ok(exit_code));
}

t_command_result	execute_env(t_state *state, t_simple *builtin)
{
	t_word_list	*arguments;
	t_string	*env;

	arguments = builtin->words->next;
	if (arguments != NULL)
	{
		write(STDERR_FILENO, TOO_MANY_ARGS, ft_strlen(TOO_MANY_ARGS));
		return (command_ok(EX_BADUSAGE));
	}
	env = string_new();
	if (!env)
		return (command_err(E_OOM));
	if (gather_environment(state->env, &env) != NO_ERROR)
	{
		string_destroy(env);
		return (command_err(E_OOM));
	}
	return (write_env_to_stdout(env));
}
