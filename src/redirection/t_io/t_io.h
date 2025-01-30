/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_io.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:53:31 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:53:32 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_IO_H
# define T_IO_H

# include "error/t_error.h"

# define DO_NOT_PIPE -1

typedef struct s_io
{
	int	input;
	int	output;
}		t_io;

t_io	io_default(void);
t_io	io_new(int input, int output);
t_io	io_from_array(int arr[2]);

void	io_close(t_io io);
t_error	do_piping(t_io io);

#endif // T_IO_H
