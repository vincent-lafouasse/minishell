/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlafouas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:40:18 by vlafouas          #+#    #+#             */
/*   Updated: 2023/11/22 00:03:51 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const t_byte	*bytes1;
	const t_byte	*bytes2;
	size_t			i;

	bytes1 = s1;
	bytes2 = s2;
	i = 0;
	while (i < n)
	{
		if (bytes1[i] != bytes2[i])
			return (bytes1[i] - bytes2[i]);
		i++;
	}
	return (0);
}
