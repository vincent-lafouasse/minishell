#include "process.h"

#include "error/t_error.h"

#include "libft/ft_io.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

static void compensate_for_lack_of_newline(void)
{
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static pid_t wait_through_signals(pid_t pid, int *status_out)
{
	pid_t waited_for_pid;

	while (true)
	{
		waited_for_pid = waitpid(pid, status_out, 0);
		if (waited_for_pid < 0 && errno == EINTR)
			continue;
		// an event other than the process exiting occured, we don't care for it
		if (waited_for_pid >= 0 && \
			(!WIFEXITED(*status_out) && !WIFSIGNALED(*status_out)))
			continue;
		break;
	}
	return (waited_for_pid);
}

static int get_exit_status(int status)
{
	if (WIFSIGNALED (status))
		return (128 + WTERMSIG (status));
	else if (WIFEXITED(status))
		return (WEXITSTATUS (status));
	else
		return (EXIT_SUCCESS);
}

void report_signal_related_exit(int status)
{
	int signal_id;

	signal_id = WTERMSIG(status);
	ft_putstr_fd("minishell: process killed by signal #", STDERR_FILENO);
	ft_putnbr_fd(signal_id, STDERR_FILENO);
	if (WCOREDUMP(status))
		ft_putstr_fd(" (core dumped)", STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

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
			compensate_for_lack_of_newline();
	}
	*exit_status_out = get_exit_status(status);
	return (NO_ERROR);
}
