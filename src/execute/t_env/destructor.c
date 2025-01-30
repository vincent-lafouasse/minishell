/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:29:53 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:30:59 by poss             ###   ########.fr       */
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

char	**destroy_envp_array(char **array, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
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
