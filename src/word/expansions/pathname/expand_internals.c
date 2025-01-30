/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_internals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:38:43 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:41:11 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "libft/string.h"

void ft_split_destroy(char *data[])
{
	size_t i;

	i = 0;
	while (data[i])
	{
		free(data[i]);
		i++;
	}
	free(data);
}

char *join_delimited(const char *s1, char delim, const char *s2)
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

bool is_absolute_pathname(const char *word)
{
	return (ft_strchr(word, '/') != (char *)NULL);
}

bool filename_is_directory(const char *command_path)
{
	struct stat command_stats;
	if (stat(command_path, &command_stats) < 0)
		return false;
	return (S_ISDIR(command_stats.st_mode));
}
