#include "t_env.h"
#include "t_env_internals.h"
#include "libft/ft_string.h"

#include <stdlib.h>

const t_env_entry	*env_get(const t_env *env, const char *key)
{
	t_env *node;

	node = env_find_node(env, key);
	if (!node)
		return NULL;
	return (&node->entry);
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
	if (!path_entry)
		return (empty_array());
	return (ft_split(path_entry->value, ':'));
}

//char				**env_make_envp(const t_env **env);
