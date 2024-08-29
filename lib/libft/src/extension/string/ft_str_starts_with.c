/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_starts_with.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:14:07 by poss              #+#    #+#             */
/*   Updated: 2024/08/17 14:15:36 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft_string.h"
#include "libft/string.h"

bool	ft_str_starts_with(const char *s, const char *pattern)
{
	if (!s || !pattern)
		return (false);
	if (ft_strlen(pattern) > ft_strlen(s))
		return (false);
	while (*pattern)
	{
		if (*pattern != *s)
			return (false);
		s++;
		pattern++;
	}
	return (true);
}
