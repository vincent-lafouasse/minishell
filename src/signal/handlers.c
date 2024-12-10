#include "signal.h"
#include <stdio.h>
#include <unistd.h>

sig_atomic_t last_signal = 0;

void install_interactive_handlers(void)
{
	install_noop_rl_event_hook();
	signal(SIGINT, save_interrupt_and_reject_line);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

void install_non_interactive_handlers(void)
{
	make_rl_return_when_input_exhausted();
	signal(SIGINT, save_interrupt_and_reject_line);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

void reset_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}

