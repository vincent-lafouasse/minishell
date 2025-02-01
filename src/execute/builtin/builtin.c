/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:23:51 by poss              #+#    #+#             */
/*   Updated: 2025/02/01 21:58:46 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"
#include "builtin.h"
#include "error/t_error.h"
#include "libft/string.h"
#include <unistd.h>

static bool			str_eq(const char *a, const char *b);

t_command_result	execute_builtin(t_state *state, t_simple *simple)
{
	const char	*command = simple->words->contents;

	if (str_eq(command, "echo"))
		return (execute_echo(state, simple));
	if (str_eq(command, "cd"))
		return (execute_cd(state, simple));
	if (str_eq(command, "pwd"))
		return (execute_pwd(state, simple));
	if (str_eq(command, "export"))
		return (execute_export(state, simple));
	if (str_eq(command, "unset"))
		return (execute_unset(state, simple));
	if (str_eq(command, "env"))
		return (execute_env(state, simple));
	if (str_eq(command, "exit"))
		return (execute_exit(state, simple));
	return (command_err(E_UNREACHABLE));
}

bool	is_builtin_command(const t_simple *simple)
{
	const char	*command;

	if (simple->words == NULL)
		return (false);
	command = simple->words->contents;
	if (str_eq(command, "echo"))
		return (true);
	if (str_eq(command, "cd"))
		return (true);
	if (str_eq(command, "pwd"))
		return (true);
	if (str_eq(command, "export"))
		return (true);
	if (str_eq(command, "unset"))
		return (true);
	if (str_eq(command, "env"))
		return (true);
	if (str_eq(command, "exit"))
		return (true);
	return (false);
}

static bool	str_eq(const char *a, const char *b)
{
	size_t	len;

	len = ft_strlen(a);
	return (ft_strncmp(a, b, len + 1) == 0);
}
