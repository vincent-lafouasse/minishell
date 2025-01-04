#include "t_error.h"

#include <stdbool.h>

bool	is_syscall_related(t_error err)
{
	return (err == E_OOM || err == E_FORK || err == E_DUP2 || \
			err == E_EXECVE || err == E_ACCESS || err == E_GETCWD || \
			err == E_WRITE || err == E_WAIT || err == E_OPEN ||	\
			err == E_UNLINK || err == E_PIPE || err == E_DUMMY);
}
