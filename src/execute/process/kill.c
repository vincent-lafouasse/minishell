#include "process.h"

#include "shell/shell.h"

#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

static void kill_pid_list(const t_pid_list *pids)
{
	const t_pid_list *curr;

	curr = pids;
	while (curr)
	{
		kill(curr->pid, SIGKILL);
		waitpid(curr->pid, NULL, WNOHANG);
		curr = curr->next;
	}
}

void	kill_pipeline(t_state *state, const t_pid_list *pids)
{
	kill_pid_list(pids);
	if (state->is_interactive)
		reset_tty_properties(STDERR_FILENO, &state->tty_properties);
}

void	kill_process(t_state *state, pid_t pid)
{
	kill(pid, SIGKILL);
	waitpid(pid, NULL, WNOHANG);
	if (state->is_interactive)
		reset_tty_properties(STDERR_FILENO, &state->tty_properties);
}
