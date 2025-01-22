/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checked_atou.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:42:12 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 16:42:13 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ctype.h"
#include "libft/string.h"
#include <stdbool.h>
#include <stdint.h>

#define STR_U32_MAX "4294967295"

static bool	str_is_all_numbers(const char *s);
static bool	will_overflow_u32(const char *s);

bool	checked_atou(const char *s, uint32_t *out)
{
	uint32_t	val;

	if (!s || !str_is_all_numbers(s) || !*s)
		return (false);
	if (*s == '0' && *(s + 1) != '\0')
		return (false);
	if (will_overflow_u32(s))
		return (false);
	val = 0;
	while (*s)
	{
		val = 10 * val + (*s - '0');
		s++;
	}
	*out = val;
	return (true);
}

static bool	str_is_all_numbers(const char *s)
{
	if (!s)
		return (false);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

static bool	will_overflow_u32(const char *s)
{
	if (ft_strlen(s) > ft_strlen(STR_U32_MAX))
		return (true);
	if (ft_strlen(s) < ft_strlen(STR_U32_MAX))
		return (false);
	return (ft_strncmp(s, STR_U32_MAX, ft_strlen(STR_U32_MAX)) > 0);
}
