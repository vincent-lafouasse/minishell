/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_env_internals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:34:31 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:42:23 by poss             ###   ########.fr       */
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

t_error	env_push_front(t_env **env, t_env_entry entry)
{
	t_env	*new_last;

	new_last = env_new(entry);
	if (new_last == NULL)
		return (E_OOM);
	env_push_front_link(env, new_last);
	return (NO_ERROR);
}

void	env_push_front_link(t_env **env, t_env *link)
{
	if (env == NULL)
		return ;
	link->next = *env;
	if (link->next)
		link->next->prev = link;
	*env = link;
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
