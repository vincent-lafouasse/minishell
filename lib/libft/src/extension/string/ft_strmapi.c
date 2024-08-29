/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:33:03 by poss              #+#    #+#             */
/*   Updated: 2023/12/04 17:28:40 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*out;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	out = malloc(1 + ft_strlen(s));
	i = 0;
	while (s[i])
	{
		out[i] = (*f)(i, s[i]);
		i++;
	}
	out[i] = 0;
	return (out);
}
