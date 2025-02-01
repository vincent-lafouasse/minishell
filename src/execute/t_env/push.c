/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:31:17 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:31:46 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_env_internals.h"
#include <stddef.h>

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
