/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_env_internals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:26:52 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:31:40 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/string.h"
#include "t_env_internals.h"
#include <stdlib.h>

t_env	*env_new(t_env_entry entry)
{
	t_env	*out;

	out = malloc(sizeof(*out));
	if (out == NULL)
		return (NULL);
	*out = (t_env){.entry = entry, .next = NULL, .prev = NULL};
	return (out);
}

t_env	*env_find_node(t_env *env, const char *key)
{
	t_env	*node;
	size_t	key_length;

	node = env;
	key_length = ft_strlen(key);
	while (node && ft_strncmp(node->entry.key, key, key_length + 1) != 0)
		node = node->next;
	return (node);
}

t_env	*env_detach_link(t_env *env, t_env **head)
{
	t_env	*prev;
	t_env	*current;
	t_env	*next;

	if (!env || !*head)
		return (NULL);
	prev = env->prev;
	current = env;
	next = env->next;
	if (prev)
		prev->next = next;
	else
		*head = next;
	if (next)
		next->prev = prev;
	current->next = NULL;
	current->prev = NULL;
	return (current);
}

t_env	*env_pop_key(t_env **env, const char *key)
{
	t_env	*key_node;

	key_node = env_find_node(*env, key);
	return (env_detach_link(key_node, env));
}
