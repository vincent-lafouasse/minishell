/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 20:03:51 by jamar             #+#    #+#             */
/*   Updated: 2024/01/31 15:43:10 by jamar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/string.h"
#include <stdlib.h>

/**
 * @brief Resizes a memory allocation.
 *
 * `ft_realloc`, libc `realloc`'s make-shift counterpart, attempts to resize an
 * existing memory allocation by allocating it again using libc `malloc`,
 * copying its contents into the new allocation and `free`ing the previous,
 * smaller allocation.
 *
 * In the event that `new_size` is greater than `len`, any memory beyond `len`
 * in the new allocation should be considered uninitialized.
 *
 * Due to libc's `realloc` having access to allocator internals, its exact
 * behaviour cannot be replicated using the subset of libc allowed at 42 and we
 * miss out on an optimization where no neighbouring allocations exist next to
 * `old` in memory.
 *
 * If `len` and `new_size` are equal, the allocation will still be resized since
 * `len` is not the size of the current buffer, but only the amount of bytes in
 * it. Any given allocation could have a larger capacity than the amount of
 * bytes it currently holds, and shrinking the allocation may be the expected
 * behaviour.
 *
 * Should the function fail, it will return `NULL` and `old` remains allocated.
 *
 * @callgraph
 *
 * @warning pointer parameter `old` must point to addressable bytes which are
 * allocated on the heap using libc's `malloc`!
 * @warning in the event of an error, `old` remains allocated!
 *
 * @param old A pointer to the current allocation.
 * @param len The amount of addressable bytes in the current allocation.
 * @param new_size The size of the new allocation.
 * @see realloc
 * @return A pointer to the new allocation on success, and `NULL` in the event
 * of an allocation error (ENOMEM).
 */
void	*ft_realloc(void *old, size_t len, size_t new_size)
{
	void	*new;

	new = malloc(new_size);
	if (!new)
		return (NULL);
	if (len != 0 && old != NULL)
	{
		if (new_size < len)
			ft_memcpy(new, old, new_size);
		else
			ft_memcpy(new, old, len);
	}
	free(old);
	return (new);
}
