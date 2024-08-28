/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlafouas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:18:48 by vlafouas          #+#    #+#             */
/*   Updated: 2023/11/22 00:06:37 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	t_byte	*dest_bytes;
	t_byte	*src_bytes;

	dest_bytes = (t_byte *)dest;
	src_bytes = (t_byte *)src;
	while (n)
	{
		*dest_bytes = *src_bytes;
		n--;
		dest_bytes++;
		src_bytes++;
	}
	return (dest);
}
