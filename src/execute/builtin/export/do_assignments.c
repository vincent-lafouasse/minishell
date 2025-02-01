/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_assignments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:20:59 by poss              #+#    #+#             */
/*   Updated: 2025/02/01 21:58:38 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin.h"
#include "error/t_error.h"
#include "execute/execute.h"
#include "execute/t_env/t_env.h"
#include "libft/ctype.h"
#include "libft/ft_string.h"
#include "libft/string.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_assignment
{
	bool			appending;
	char			*name;
	char			*value;
}					t_assignment;

void	clean_up_assignment(t_assignment *assignment)
{
	free(assignment->name);
	free(assignment->value);
	assignment->name = NULL;
	assignment->value = NULL;
}

// returns 0 if assignment is malformated in any way
static size_t	name_len(char *assignment)
{
	size_t	i;

	if (!ft_isalpha(*assignment) && *assignment != '_')
		return (0);
	i = 1;
	while (assignment[i])
	{
		if (assignment[i] == '=')
			return (i);
		if (assignment[i] == '+' && assignment[i + 1] == '=')
			return (i);
		if (!ft_isalnum(assignment[i]) && assignment[i] != '_')
			return (0);
		i++;
	}
	return (i);
}

static t_error	parse_assignment(char *str, t_assignment *out)
{
	size_t	len;

	*out = (t_assignment){0};
	len = name_len(str);
	if (len == 0)
		return (NO_ERROR);
	out->name = ft_substr(str, 0, len);
	if (!out->name)
		return (E_OOM);
	if (str[len] == '\0')
		return (NO_ERROR);
	out->appending = str[len] == '+';
	if (out->appending)
		out->value = ft_strdup(&str[len + 2]);
	else
		out->value = ft_strdup(&str[len + 1]);
	if (!out->value)
	{
		free(out->name);
		out->name = NULL;
		return (E_OOM);
	}
	return (NO_ERROR);
}

static t_error	assign_variable(t_env **env, t_assignment assignment)
{
	t_env_entry	*entry;
	char		*joined;

	if (!env_key_exists(*env, assignment.name))
		return (env_insert_owned_kv(env, assignment.name, assignment.value));
	if (!assignment.value)
		return (NO_ERROR);
	entry = env_get_mut(*env, assignment.name);
	if (assignment.appending)
	{
		if (entry->value)
			joined = ft_strjoin(entry->value, assignment.value);
		else
			joined = ft_strdup(assignment.value);
		if (!joined)
			return (E_OOM);
		free(assignment.value);
		assignment.value = joined;
	}
	free(entry->value);
	free(assignment.name);
	entry->value = assignment.value;
	return (NO_ERROR);
}

t_command_result	do_assignments(t_env **env, t_word_list *assignments)
{
	int				exit_code;
	t_assignment	assignment;
	t_error			err;

	exit_code = EXIT_SUCCESS;
	while (assignments)
	{
		err = parse_assignment(assignments->contents, &assignment);
		if (err != NO_ERROR)
			return (command_err(err));
		if (!assignment.name)
		{
			exit_code = EXIT_FAILURE;
			report_error(assignments->contents, "not a valid identifier");
			assignments = assignments->next;
			continue ;
		}
		err = assign_variable(env, assignment);
		if (err != NO_ERROR)
			return (clean_up_assignment(&assignment), command_err(err));
		assignments = assignments->next;
	}
	return (command_ok(exit_code));
}
