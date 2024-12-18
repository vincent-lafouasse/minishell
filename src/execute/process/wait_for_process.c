#include "process.h"

#include "error/t_error.h"

#include "libft/ft_io.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

t_error wait_for_process(pid_t pid, int *exit_status_out)
{
	int status;
	pid_t waited_for_pid;

	waited_for_pid = wait_through_signals(pid, &status);
	if (waited_for_pid < 0)
		return (E_WAIT);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) != SIGINT && WTERMSIG(status) != SIGPIPE)
			report_signal_related_exit(status);
		else if (WTERMSIG(status) == SIGINT)
			// this is normally only done when job control is enabled for bash
			// but do it anyway since it's cleaner and easier to justify in
			// defense
			ft_putchar_fd('\n', STDOUT_FILENO);
	}
	*exit_status_out = get_exit_status(status);
	return (NO_ERROR);
}
