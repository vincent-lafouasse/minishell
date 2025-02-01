/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:08:36 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 21:56:26 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal.h"
#include <unistd.h>

sig_atomic_t	g_last_signal = 0;

void	install_interactive_handlers(void)
{
	install_noop_rl_event_hook();
	signal(SIGINT, save_interrupt_and_reject_line);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

void	install_non_interactive_handlers(void)
{
	make_rl_return_when_input_exhausted();
	signal(SIGINT, save_interrupt_and_reject_line);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

void	reset_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}
