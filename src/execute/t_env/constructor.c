/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:26:32 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:26:34 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/t_error.h"
#include "libft/string.h"
#include "t_env.h"
#include "t_env_internals.h"
#include <stddef.h>
#include <stdlib.h>

static void	env_entry_destroy(t_env_entry *entry)
{
	free((void *)entry->key);
	free((void *)entry->value);
	entry->key = NULL;
	entry->value = NULL;
}

static t_error	assign_envp_value(t_env **env, char *assignment)
{
	char	*equals;
	t_error	err;

	equals = ft_strchr(assignment, '=');
	if (!equals || assignment == equals)
		return (E_MALFORMED_ENVP);
	*equals = '\0';
	err = env_insert(env, assignment, equals + 1);
	*equals = '=';
	return (err);
}

t_error	from_envp(char *values[], t_env **out)
{
	size_t	i;
	t_error	err;

	*out = NULL;
	i = 0;
	while (values[i])
	{
		err = assign_envp_value(out, values[i]);
		if (err != NO_ERROR)
		{
			env_destroy(out);
			return (err);
		}
		i++;
	}
	return (NO_ERROR);
}

t_error	env_insert(t_env **env, const char *key, const char *value)
{
	t_error	err;
	char	*owned_key;
	char	*owned_value;

	owned_key = ft_strdup(key);
	if (!owned_key)
		return (E_OOM);
	owned_value = ft_strdup(value);
	if (!owned_value)
	{
		free(owned_key);
		return (E_OOM);
	}
	err = env_insert_owned_kv(env, owned_key, owned_value);
	if (err != NO_ERROR)
	{
		free(owned_key);
		free(owned_value);
	}
	return (err);
}

t_error	env_insert_owned_kv(t_env **env, char *key, char *value)
{
	t_env_entry	entry;
	t_env_entry	*existing_entry;

	entry = (t_env_entry){.key = key, .value = value};
	if (env_key_exists(*env, key))
	{
		existing_entry = env_get_mut(*env, key);
		env_entry_destroy(existing_entry);
		*existing_entry = entry;
		return (NO_ERROR);
	}
	return (env_push_front(env, entry));
}
