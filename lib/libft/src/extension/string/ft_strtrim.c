/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:28:59 by poss              #+#    #+#             */
/*   Updated: 2023/12/21 16:05:58 by vlafouas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_n_leading_chars(char const *s1, char const *set);
static size_t	get_n_trailing_chars(char const *s1, char const *set);

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	offset;
	size_t	len;

	offset = get_n_leading_chars(s1, set);
	if (offset == ft_strlen(s1))
		return (ft_strdup(""));
	len = ft_strlen(s1) - offset - get_n_trailing_chars(s1, set);
	return (ft_substr(s1, offset, len));
}

static size_t	get_n_leading_chars(char const *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (s1[i] && ft_strchr(set, s1[i]) != NULL)
	{
		i++;
	}
	return (i);
}

static size_t	get_n_trailing_chars(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(s1);
	i = 0;
	while (i < len && ft_strchr(set, s1[len - 1 - i]) != NULL)
	{
		i++;
	}
	return (i);
}
