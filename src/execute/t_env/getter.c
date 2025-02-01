/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:27:09 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:29:18 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
