#include "libft/string.h"
#include "libft/ft_string.h"

#include "error/t_error.h"
#include "t_env.h"
#include "t_env_internals.h"

#include <stdlib.h>
#include <stddef.h>

static void env_entry_destroy(t_env_entry *entry)
{
	free((void *)entry->key);
	free((void *)entry->value);
	entry->key = NULL;
	entry->value = NULL;
}

static t_error decompose_envp_value(const char *value, t_env_entry *entry)
{
	char	*equals;

	equals = ft_strchr(value, '=');
	if (!equals || value == equals)
		return (E_MALFORMED_ENVP);
	entry->key = ft_substr(value, 0, equals - value);
	if (!entry->key)
		return (E_OOM);
	entry->value = ft_strdup(equals + 1);
	if (!entry->value)
	{
		env_entry_destroy(entry);
		return (E_OOM);
	}
	return (NO_ERROR);
}

t_error	from_envp(const char *values[], t_env **out)
{
	size_t	i;
	t_error	err;
	t_env_entry	entry;

	*out = NULL;
	i = 0;
	while (values[i])
	{
		err = decompose_envp_value(values[i], &entry);
		if (err != NO_ERROR)
		{
			env_destroy(out);
			return (err);
		}
		err = env_push_front(out, entry);
		if (err != NO_ERROR)
		{
			env_entry_destroy(&entry);
			env_destroy(out);
			return (err);
		}
		i++;
	}
	return (NO_ERROR);
}

t_error env_insert(t_env **env, const char *key, const char *value)
{
	char *owned_key, *owned_value;

	owned_key = ft_strdup(key);
	if (!owned_key)
		return (E_OOM);
	owned_value = ft_strdup(value);
	if (!owned_value)
	{
		free(owned_key);
		return (E_OOM);
	}
	return (env_insert_owned_kv(env, owned_key, owned_value));
}

t_error	env_insert_owned_kv(t_env **env, char *key, char *value)
{
	t_env_entry entry;
	t_env_entry *existing_entry;

	entry = (t_env_entry) {.key = key, .value = value};
	if (env_key_exists(*env, key))
	{
		existing_entry = env_get_mut(*env, key);
		env_entry_destroy(existing_entry);
		*existing_entry = entry;
		return (NO_ERROR);
	}
	return (env_push_front(env, entry));
}
