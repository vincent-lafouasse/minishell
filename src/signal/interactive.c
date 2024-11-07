#include "signal.h"
#include <stdio.h>
#include <unistd.h>

sig_atomic_t last_signal = 0;

static void catch_sigint(int signal)
{
	last_signal = signal;
	reject_readline_current_line();
}

void install_interactive_handlers(void)
{
	install_noop_rl_event_hook();
	signal(SIGINT, catch_sigint);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

void reset_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}

