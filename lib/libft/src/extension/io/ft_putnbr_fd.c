/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlafouas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:03:56 by vlafouas          #+#    #+#             */
/*   Updated: 2023/11/07 15:03:57 by vlafouas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_put_unsigned_fd(unsigned int n, int fd);

void	ft_putnbr_fd(int n, int fd)
{
	if (n >= 0)
		ft_put_unsigned_fd(n, fd);
	else
	{
		ft_putchar_fd('-', fd);
		ft_put_unsigned_fd(-n, fd);
	}
}

static void	ft_put_unsigned_fd(unsigned int n, int fd)
{
	if (n < 10)
		ft_putchar_fd(n + '0', fd);
	else
	{
		ft_put_unsigned_fd(n / 10, fd);
		ft_put_unsigned_fd(n % 10, fd);
	}
}
