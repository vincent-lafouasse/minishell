/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_destroy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:40:20 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:40:35 by poss             ###   ########.fr       */
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
