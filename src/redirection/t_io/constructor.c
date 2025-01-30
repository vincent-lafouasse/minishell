/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:53:26 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:53:28 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_io.h"

t_io	io_default(void)
{
	return ((t_io){DO_NOT_PIPE, DO_NOT_PIPE});
}

t_io	io_new(int input, int output)
{
	return ((t_io){.input = input, .output = output});
}

t_io	io_from_array(int arr[2])
{
	return ((t_io){.input = arr[0], .output = arr[1]});
}
