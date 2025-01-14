#include "process.h"

#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

void	attempt_to_reap_processes()
{
	int status;

	while (1)
	{
		status = waitpid(-1, NULL, WNOHANG);
		if (status == -1 && errno != EINTR)
			break;
	}
	if (status == -1 && errno != ECHILD && errno != EINTR)
		perror("minishell: kill_pipeline: waitpid");
}

void kill_pid_list(const t_pid_list *pids)
{
	const t_pid_list *curr;

	curr = pids;
	while (curr)
	{
		kill(curr->pid, SIGKILL); // BAD: bash kills with SIGTERM followed by SIGCONT
		curr = curr->next;
	}
}

void	kill_pipeline(t_state *state, const t_pid_list *pids)
{
	attempt_to_reap_processes();
	kill_pid_list(pids);
	if (state->is_interactive)
		reset_tty_properties(STDERR_FILENO, &state->tty_properties);
}

void	kill_process(t_state *state, pid_t pid)
{
	attempt_to_reap_processes();
	kill(pid, SIGKILL);
	if (state->is_interactive)
		reset_tty_properties(STDERR_FILENO, &state->tty_properties);
}
