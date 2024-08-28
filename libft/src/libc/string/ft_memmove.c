/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 22:20:13 by poss              #+#    #+#             */
/*   Updated: 2023/11/22 00:11:14 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* this function was made to deal with overlap.
 * there are 2 cases for overlap:
 * - overlap at the end of src        -> copy backward
 * - overlap at the beginnning of src -> copy forward (i.e. memcpy)
 *
 * hence there is only one special case : overlap at the end of src
 * i.e. dest is in [src, src + n[
 * i.e. all normal cases are for dest not in [src, src + n[
 * i.e. dest < src || dest >= src + n
 */

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	t_byte			*dest_bytes;
	const t_byte	*src_bytes;

	dest_bytes = (t_byte *)dest;
	src_bytes = (t_byte *)src;
	if (dest < src || dest >= src + n)
	{
		return (ft_memcpy(dest, src, n));
	}
	while (n)
	{
		dest_bytes[n - 1] = src_bytes[n - 1];
		n--;
	}
	return (dest);
}
