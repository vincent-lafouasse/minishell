/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_tty_properties.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:48:54 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:48:56 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft_io.h"
#include "process.h"
#include <termios.h>

#define TCSETATTR_ERROR "minishell: tcsetattr: could not reset tty properties\n"

void	reset_tty_properties(int tty_fd, struct termios *tty_properties)
{
	if (tcsetattr(tty_fd, TCSADRAIN, tty_properties) < 0)
		ft_putendl_fd(TCSETATTR_ERROR, STDERR_FILENO);
}
