/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:37:48 by poss              #+#    #+#             */
/*   Updated: 2023/12/01 16:55:39 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void			reverse_fill(char *buffer, int n);
void			mem_reverse(void *s, size_t len);
static size_t	get_n_digits(unsigned int n);
unsigned int	ft_abs(int n);

char	*ft_itoa(int n)
{
	char	*out;
	size_t	len;

	if (n == 0)
		return (ft_strdup("0"));
	len = get_n_digits(ft_abs(n)) + (n < 0);
	out = malloc(1 + len);
	if (!out)
		return (NULL);
	reverse_fill(out, n);
	mem_reverse(out, len);
	return (out);
}

void	reverse_fill(char *buffer, int n)
{
	unsigned int	abs;

	abs = ft_abs(n);
	while (abs)
	{
		*buffer++ = '0' + (abs % 10);
		abs /= 10;
	}
	if (n < 0)
		*buffer++ = '-';
	*buffer = 0;
}

void	mem_reverse(void *s, size_t len)
{
	size_t	i;
	t_byte	mem;
	t_byte	*bytes;

	i = 0;
	bytes = s;
	while (i < len / 2)
	{
		mem = bytes[i];
		bytes[i] = bytes[len - 1 - i];
		bytes[len - 1 - i] = mem;
		i++;
	}
}

static size_t	get_n_digits(unsigned int n)
{
	size_t	n_digits;

	n_digits = 0;
	while (n)
	{
		n /= 10;
		n_digits++;
	}
	return (n_digits);
}

unsigned int	ft_abs(int n)
{
	if (n >= 0)
		return (n);
	else
		return (-n);
}
