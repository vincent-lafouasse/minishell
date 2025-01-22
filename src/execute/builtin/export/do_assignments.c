/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_assignments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:48:20 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:16:48 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin.h"
#include "error/t_error.h"
#include "execute/execute.h"
#include "execute/t_env/t_env.h"
#include "export_internals.h"
#include "libft/ft_string.h"
#include "libft/string.h"
#include <assert.h> // temporarily
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_assignment
{
	bool			appending;
	char			*name;
	char			*value;
}					t_assignment;

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
	{
		out->value = NULL;
		out->appending = false;
	}
	else if (str[len] == '+')
	{
		out->value = ft_strdup(&str[len + 2]);
		if (!out->value)
		{
			free(out->name);
			out->name = NULL;
			return (E_OOM);
		}
		out->appending = true;
	}
	else if (str[len] == '=')
	{
		out->value = ft_strdup(&str[len + 1]);
		if (!out->value)
		{
			free(out->name);
			out->name = NULL;
			return (E_OOM);
		}
		out->appending = false;
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
	assert(entry != NULL);
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

static t_error	process_assignment(char *s, t_env **env, bool *any_failed)
{
	t_assignment	assignment;
	t_error			err;

	err = parse_assignment(s, &assignment);
	if (err != NO_ERROR)
		return (err);
	if (!assignment.name)
	{
		*any_failed = true;
		report_invalid_identifier(s);
		return (NO_ERROR);
	}
	err = assign_variable(env, assignment);
	if (err != NO_ERROR)
	{
		free(assignment.name);
		free(assignment.value);
		return (err);
	}
	return (NO_ERROR);
}

t_command_result	do_assignments(t_env **env, t_word_list *assignments)
{
	bool	any_failed;
	t_error	err;

	any_failed = false;
	while (assignments)
	{
		err = process_assignment(assignments->contents, env, &any_failed);
		if (err != NO_ERROR)
			return ((t_command_result){.error = err});
		assignments = assignments->next;
	}
	if (any_failed)
		return ((t_command_result){.error = NO_ERROR,
			.status_code = EXIT_FAILURE});
	else
		return ((t_command_result){.error = NO_ERROR,
			.status_code = EXIT_SUCCESS});
}
