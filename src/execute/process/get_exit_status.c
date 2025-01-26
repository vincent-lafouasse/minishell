#include "process.h"

#include <stdlib.h>
#include <sys/wait.h>

int get_exit_status(int status)
{
	if (WIFSIGNALED (status))
		return (128 + WTERMSIG (status));
	else if (WIFEXITED(status))
		return (WEXITSTATUS (status));
	else
		return (EXIT_SUCCESS);
}
