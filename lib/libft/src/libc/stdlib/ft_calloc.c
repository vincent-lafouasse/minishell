/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlafouas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:12:21 by vlafouas          #+#    #+#             */
/*   Updated: 2023/12/04 18:01:20 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

/* * includes a sanity check to ensure that there was no overflow
 */

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*output;

	if (size != 0 && (nmemb * size) / size != nmemb)
		return (NULL);
	output = malloc(nmemb * size);
	if (output == NULL)
		return (NULL);
	ft_bzero(output, nmemb * size);
	return (output);
}
