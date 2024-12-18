#include "process.h"

#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

pid_t wait_through_signals(pid_t pid, int *status_out)
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
