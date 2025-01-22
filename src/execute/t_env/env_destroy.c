/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_destroy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:40:20 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:42:16 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_env_internals.h"

#include <stdlib.h>

t_env	*env_remove(t_env **env, const char *key)
{
	return (env_pop_key(env, key));
}

void	env_destroy(t_env **env)
{
	env_clear(env, free);
}

t_env	*env_pop_key(t_env **env, const char *key)
{
	t_env	*key_node;

	key_node = env_find_node(*env, key);
	return (env_detach_link(key_node, env));
}

void	env_delone(t_env **env, t_destructor del)
{
	t_env	*new_head;

	if (!env || !*env)
		return ;
	new_head = (*env)->next;
	if (new_head)
		new_head->prev = NULL;
	if (del)
	{
		del((void *)(*env)->entry.key);
		del((void *)(*env)->entry.value);
	}
	free(*env);
	*env = new_head;
}

void	env_clear(t_env **env, t_destructor del)
{
	if (!env || !*env)
		return ;
	while ((*env)->prev)
		*env = (*env)->prev;
	while (*env)
		env_delone(env, del);
}
