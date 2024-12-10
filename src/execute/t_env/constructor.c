#include "libft/string.h"
#include "t_env.h"
#include "t_env_internals.h"
#include "error/t_error.h"
#include "libft/ft_string.h"

#include <stdlib.h>
#include <stddef.h>

static void env_entry_destroy(t_env_entry *entry)
{
	free((void *)entry->key);
	free((void *)entry->value);
}

static t_error decompose_envp_value(const char *value, t_env_entry *entry)
{
	char	*equals;

	equals = ft_strchr(value, '=');
	if (!equals)
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

t_error	env_insert_owned_kv(t_env **env, char *key, char *value)
{
	t_env_entry entry;
	t_env_entry *existing_entry;

	entry = (t_env_entry) {.key = key, .value = value};
	if (env_key_exists(env, key))
	{
		existing_entry = env_get_mut(env, key);
		env_entry_destroy(existing_entry);
		*existing_entry = entry;
		return (NO_ERROR);
	}
	return (env_push_front(env, entry));
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
			env_clear(out, NULL); // bad, should destroy entry key and value
			return (err);
		}
		err = env_push_front(out, entry);
		if (err != NO_ERROR)
		{
			env_entry_destroy(&entry);
			env_clear(out, NULL); // bad, should destroy entry key and value
			return (err);
		}
		i++;
	}
	return (NO_ERROR);
}
