/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlafouas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:57:06 by vlafouas          #+#    #+#             */
/*   Updated: 2023/12/04 18:00:49 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_islower(int c);

int	ft_toupper(int c)
{
	if (!ft_islower(c))
		return (c);
	return (c - 32);
}

static int	ft_islower(int c)
{
	return (c >= 'a' && c <= 'z');
}
