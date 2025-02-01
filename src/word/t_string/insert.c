/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 19:04:44 by jamar             #+#    #+#             */
/*   Updated: 2025/01/30 19:33:59 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_string.h"
#include "libft/string.h"
#include <stddef.h>

t_oom	string_push(t_string **self, char t)
{
	if ((*self)->len == (*self)->cap)
	{
		if (string_reserve(self, 1) == true)
			return (true);
	}
	(*self)->data[(*self)->len] = t;
	(*self)->len += 1;
	return (false);
}

t_oom	string_extend(t_string **self, const char *other)
{
	size_t	string_len;

	string_len = ft_strlen(other);
	return (string_extend_n(self, other, string_len));
}

t_oom	string_extend_n(t_string **self, const char *other, size_t n)
{
	if (string_reserve(self, n) == true)
		return (true);
	ft_memcpy(&(*self)->data[(*self)->len], other, n);
	(*self)->len += n;
	return (false);
}
