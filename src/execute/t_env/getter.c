/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:34:09 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:40:40 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft_string.h"
#include "libft/stdlib.h"
#include "libft/string.h"
#include "t_env.h"
#include "t_env_internals.h"
#include <stdlib.h>

const t_env_entry	*env_get(const t_env *env, const char *key)
{
	t_env	*node;

	node = env_find_node((t_env *)env, key);
	if (!node)
		return (NULL);
	return (&node->entry);
}

t_env_entry	*env_get_mut(t_env *env, const char *key)
{
	t_env	*node;

	node = env_find_node((t_env *)env, key);
	if (!node)
		return (NULL);
	return (&node->entry);
}

bool	env_key_exists(const t_env *env, const char *key)
{
	return (env_get(env, key) != NULL);
}

static char	**empty_array(void)
{
	char	**out;

	out = malloc(sizeof(char *));
	*out = NULL;
	return (out);
}

char	**env_make_path_or_empty(const t_env *env)
{
	const t_env_entry	*path_entry;

	path_entry = env_get(env, "PATH");
	if (!path_entry || !path_entry->value || *path_entry->value == '\0')
		return (empty_array());
	return (ft_split(path_entry->value, ':'));
}
