#include "process.h"

#include "../t_pid_list/t_pid_list.h"
#include "error/t_error.h"

#include "libft/ft_io.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

t_error reap_remaining_pipeline_processes(void)
{
	while (true)
	{
		pid_t pid = wait_through_signals(-1, NULL);
		if (pid >= 0)
			continue;
		if (errno != ECHILD)
			return (E_WAIT);
		// pid < 0 && errno == ECHILD after `wait(-1)`: we have no more children
		break;
	}
	return (NO_ERROR);
}

t_error wait_for_pipeline(t_pid_list *pids, int *last_exit_status_out)
{
	pid_t last_pid;
	int last_status;

	if (!pids)
	{
		*last_exit_status_out = 0;
		return (NO_ERROR);
	}
	last_pid = pidl_last(pids)->pid;
	if (wait_through_signals(last_pid, &last_status) < 0)
		// remaining child processes should be killed by the caller, THOUGH:
		return (E_WAIT); // BAD: `kill(2)` has no effect on zombie processes, so `wait(pid, WNOHANG)` them first
	if (WIFSIGNALED(last_status))
	{
		if (WTERMSIG(last_status) != SIGINT && WTERMSIG(last_status) != SIGPIPE)
			report_signal_related_exit(last_status);
		else if (WTERMSIG(last_status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
	}
	*last_exit_status_out = get_exit_status(last_status);
	return (reap_remaining_pipeline_processes());
}
