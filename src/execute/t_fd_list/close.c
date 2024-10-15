#include "t_fd_list.h"

#include <unistd.h>

void fdl_close_and_clear(t_fd_list **fds_to_close)
{
	int	fd;

	if (!fds_to_close)
		return ;

	while (*fds_to_close)
	{
		fd = fdl_pop_front(fds_to_close);
		if (fd > 0)
			close(fd);
	}
}
