/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlafouas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:36:35 by vlafouas          #+#    #+#             */
/*   Updated: 2024/10/07 17:14:35 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_stripped_to_unsigned(const char *num_str);

int	ft_atoi(const char *num_str)
{
	char	sign;

	sign = 1;
	while (ft_isspace(*num_str))
	{
		num_str++;
	}
	if (*num_str == '-')
	{
		sign = -1;
		num_str++;
	}
	else if (*num_str == '+')
	{
		num_str++;
	}
	return (sign * ft_stripped_to_unsigned(num_str));
}

static unsigned int	ft_stripped_to_unsigned(const char *num_str)
{
	unsigned int	output;

	output = 0;
	while (ft_isdigit(*num_str))
	{
		output = 10 * output + (*num_str - '0');
		num_str++;
	}
	return (output);
}
