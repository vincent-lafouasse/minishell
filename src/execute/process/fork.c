#include "process.h"

#include "execute/execute.h"
#include "error/t_error.h"

#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

t_error fork_process(bool *in_child, pid_t *pid_out)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		return E_FORK;
	if (in_child != NULL)
		*in_child = pid == 0;
	if (pid_out != NULL && pid == 0)
		*pid_out = pid;
	return (NO_ERROR);
}

t_error fork_and_push_pid(bool *in_child, t_pid_list **pidl_out)
{
	t_error err;
	t_pid_list  *link;

	// it is cheaper to clean up one allocation than it is to clean up a fork in
	// the event of an error; optimistically allocate a link here
	link = pidl_new(0);
	if (!link)
		return (E_OOM);
	err = fork_process(in_child, &link->pid);
	if (err != NO_ERROR)
	{
		pidl_clear(&link);
		return err;
	}
	if (*in_child)
		pidl_clear(&link);
	else
		pidl_push_back_link(pidl_out, link);
	return (NO_ERROR);
}
