/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:38:18 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:41:18 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/stdlib.h"
#include "libft/string.h"
#include "t_env_internals.h"
#include <stdlib.h>

static char		*join_delimited(const char *s1, char delim, const char *s2);
static size_t	env_entry_count(const t_env *env);

// bad, `t_env/make_envp.c` no cleanup
char	**env_make_envp(const t_env *env)
{
	size_t	entry_count;
	char	**out;
	char	*joined_entry;
	size_t	i;

	entry_count = env_entry_count(env);
	out = ft_calloc(entry_count + 1, sizeof(*out));
	if (!out)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->entry.value)
		{
			joined_entry = join_delimited(env->entry.key, '=',
					env->entry.value);
			if (!joined_entry)
				__builtin_trap();
			out[i] = joined_entry;
			i++;
		}
		env = env->next;
	}
	return (out);
}

static size_t	env_entry_count(const t_env *env)
{
	size_t	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

static char	*join_delimited(const char *s1, char delim, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*out;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	out = malloc(len1 + 1 + len2 + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, s1, len1);
	out[len1] = delim;
	ft_memcpy(out + len1 + 1, s2, len2);
	out[len1 + 1 + len2] = '\0';
	return (out);
}
