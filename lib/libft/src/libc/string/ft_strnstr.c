/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:24:46 by poss              #+#    #+#             */
/*   Updated: 2023/12/04 17:35:33 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	needle_len;

	needle_len = ft_strlen(needle);
	if (needle_len == 0)
	{
		return ((char *)haystack);
	}
	i = 0;
	while (*haystack && i + needle_len <= len)
	{
		if (!ft_strncmp(haystack, needle, needle_len))
		{
			return ((char *)haystack);
		}
		i++;
		haystack++;
	}
	return (NULL);
}
