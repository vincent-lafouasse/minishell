#include "process.h"

#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

pid_t wait_through_signals(pid_t pid, int *status_out)
{
	pid_t waited_for_pid;
	int status;

	while (true)
	{
		waited_for_pid = waitpid(pid, &status, 0);
		if (waited_for_pid < 0 && errno == EINTR)
			continue;
		if (waited_for_pid >= 0)
		{
			// an event other than a process exiting occured, we don't care for it
			if (!WIFEXITED(status) && !WIFSIGNALED(status))
				continue;
			// we have caught the termination of a different process than the
			// one we were looking for; bash would save this to its global job
			// list to keep track of it but since we can't make global
			// variables, we have to be careful not to leave any processes
			// unaccounted for
			if (pid != -1 && waited_for_pid != pid)
				continue;
		}
		break;
	}
	if (waited_for_pid >= 0 && status_out)
		*status_out = status;
	return (waited_for_pid);
}
