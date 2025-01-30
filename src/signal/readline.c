/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:09:00 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 21:56:50 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal.h"
#include <stdio.h>
#include <readline/readline.h>

static int	do_nothing(void)
{
	return (0);
}

// `readline` does not respond to changes done to the `rl_done` variable unless
// `rl_event_hook` is non-NULL
void	install_noop_rl_event_hook(void)
{
	rl_event_hook = do_nothing;
}

static void	reject_readline_current_line(void)
{
	rl_done = 1;
}

void	save_interrupt_and_reject_line(int signal)
{
	g_last_signal = signal;
	reject_readline_current_line();
}
