/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_tty_properties.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:35:57 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:39:28 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/t_error.h"
#include "process.h"

void	reset_tty_properties(int tty_fd, struct termios *tty_properties)
{
	if (tcsetattr(tty_fd, TCSADRAIN, tty_properties) < 0)
		report_error("tcsetattr", "could not reset tty properties");
}
