#include "signal.h"
#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>

sig_atomic_t last_signal = 0;

static void clear_current_line(int signal)
{
	(void)signal;
	int clear_undo = 0; // bad, ????

	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", clear_undo);
	rl_on_new_line();
	rl_redisplay();
}

static void save_signal(int signal)
{
	//printf("received signal: %d\n", signal);
	last_signal = signal;
}

void install_interactive_handlers()
{
	signal(SIGINT, clear_current_line);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

