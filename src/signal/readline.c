#include "signal.h"
#include <stdio.h>
#include <readline/readline.h>

static int do_nothing(void)
{
	return (0);
}

// HACK: `readline` does not respond to changes done to the `rl_done` variable
// unless `rl_event_hook` is non-NULL
void install_noop_rl_event_hook(void)
{
	rl_event_hook = do_nothing; // XXX: this makes readline keep trying to read in a loop if stdin is not a tty and closes
}

static void reject_readline_current_line(void)
{
	rl_done = 1;
}

void save_interrupt_and_reject_line(int signal)
{
	last_signal = signal;
	reject_readline_current_line();
}
