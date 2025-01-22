/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report_signal_related_exit.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:48:50 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:48:52 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft_io.h"
#include "process.h"
#include <sys/wait.h>
#include <unistd.h>

void	report_signal_related_exit(int status)
{
	int	signal_id;

	signal_id = WTERMSIG(status);
	ft_putstr_fd("minishell: process killed by signal #", STDERR_FILENO);
	ft_putnbr_fd(signal_id, STDERR_FILENO);
	if (WCOREDUMP(status))
		ft_putstr_fd(" (core dumped)", STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}
