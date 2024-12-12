#ifndef SIGNAL_H
# define SIGNAL_H

#include <signal.h>

extern sig_atomic_t last_signal;

void install_interactive_handlers(void);
void install_non_interactive_handlers(void);
void reset_signal_handlers(void);

// internal

void install_noop_rl_event_hook(void);
void save_interrupt_and_reject_line(int signal);
void make_rl_return_when_input_exhausted(void);

#endif // SIGNAL_H
