#include "t_env.h"
#include "t_env_internals.h"
#include "libft/ft_string.h"
#include "libft/string.h"
#include "libft/stdlib.h"

#include <stdlib.h>

const t_env_entry	*env_get(const t_env *env, const char *key)
{
	t_env *node;

	node = env_find_node((t_env*)env, key);
	if (!node)
		return NULL;
	return (&node->entry);
}

t_env_entry	*env_get_mut(t_env *env, const char *key)
{
	t_env *node;

	node = env_find_node((t_env*)env, key);
	if (!node)
		return NULL;
	return (&node->entry);
}

bool	env_key_exists(const t_env *env, const char *key)
{
	return env_get(env, key) != NULL;
}

static char	**empty_array(void)
{
	char **out;

	out = malloc(sizeof (char *));
	*out = NULL;
	return (out);
}

char	**env_make_path_or_empty(const t_env *env)
{
	const t_env_entry *path_entry;

	path_entry = env_get(env, "PATH");
	if (!path_entry || !path_entry->value || *path_entry->value == '\0')
		return (empty_array());
	return (ft_split(path_entry->value, ':'));
}

static char *join_delimited(const char *s1, char delim, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*out;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	out = malloc(len1 + 1 + len2 + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, s1, len1);
	out[len1] = delim;
	ft_memcpy(out + len1 + 1, s2, len2);
	out[len1 + 1 + len2] = '\0';
	return (out);
}

size_t env_entry_count(const t_env *env)
{
	size_t i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

char	**env_make_envp(const t_env *env)
{
	size_t entry_count;
	char **out;
	char *joined_entry;
	size_t i;

	entry_count = env_entry_count(env);
	out = ft_calloc(entry_count + 1, sizeof(*out));
	if (!out)
		return (NULL);
	i = 0;
	while (env) // TODO: does environment entry order matter?
	{
		if (env->entry.value)
		{
			joined_entry = join_delimited(env->entry.key, '=', env->entry.value);
			if (!joined_entry)
				__builtin_trap(); // bad, should free `out` in case of malloc error
			out[i] = joined_entry;
			i++;
		}
		env = env->next;
	}
	return (out);
}

t_env	*env_remove(t_env **env, const char *key)
{
	return env_pop_key(env, key);
}

void	env_destroy(t_env **env)
{
	env_clear(env, free);
}
