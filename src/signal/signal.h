#ifndef SIGNAL_H
# define SIGNAL_H

#include <signal.h>

extern sig_atomic_t last_signal;

void install_interactive_handlers();

#endif // SIGNAL_H
