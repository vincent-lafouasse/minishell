/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:11:47 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:12:36 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.h"
#include <stdio.h>

void	log_env(const t_env *env)
{
	t_env_entry	entry;

	printf("environment:\n");
	while (env != NULL)
	{
		entry = env->entry;
		printf("  > %s = %s\n", entry.key, entry.value);
		env = env->next;
	}
}
