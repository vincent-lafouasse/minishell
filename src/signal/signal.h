#ifndef SIGNAL_H
# define SIGNAL_H

#include <signal.h>

extern sig_atomic_t last_signal;

void install_interactive_handlers(void);
void reset_signal_handlers(void);

// internal

void install_noop_rl_event_hook(void);
void reject_readline_current_line(void);

#endif // SIGNAL_H
