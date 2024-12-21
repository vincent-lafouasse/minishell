#include "process.h"

#include "../t_pid_list/t_pid_list.h"
#include "error/t_error.h"

#include "libft/ft_io.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

static t_error waitpid_and_exhaust_children(pid_t pid, int *status_out)
{
	pid_t waited_for_pid;
	bool children_remain;
	int status;

	children_remain = true;
	while (children_remain)
	{
		waited_for_pid = wait_through_signals(-1, &status);
		if (waited_for_pid < 0)
		{
			if (errno == ECHILD) // pid < 0 && errno == ECHILD after `wait(-1)`: we have no more children
				children_remain = false;
			else
				return (E_WAIT);
		}
		if (waited_for_pid == pid)
			*status_out = status; // BAD: uninitialized read if this is never reached
	}
	return (NO_ERROR);
}

t_error wait_for_pipeline(t_pid_list *pids, int *last_exit_status_out)
{
	pid_t last_pid;
	int last_status;
	t_error err;

	if (!pids)
	{
		*last_exit_status_out = 0;
		return (NO_ERROR);
	}
	last_pid = pidl_last(pids)->pid;
	err = waitpid_and_exhaust_children(last_pid, &last_status);
	if (err != NO_ERROR)
		// E_WAIT occured, remaining child processes should be killed by the caller, THOUGH:
		return (err); // BAD: `kill(2)` has no effect on zombie processes, so `wait(pid, WNOHANG)` them first
	if (WIFSIGNALED(last_status))
	{
		if (WTERMSIG(last_status) != SIGINT && WTERMSIG(last_status) != SIGPIPE)
			report_signal_related_exit(last_status);
		else if (WTERMSIG(last_status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
	}
	*last_exit_status_out = get_exit_status(last_status);
	return (NO_ERROR);
}
