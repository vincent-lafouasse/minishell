#include "t_fd_list.h"

#include <unistd.h>

void fdl_close_and_clear(t_fd_list **fdl)
{
	int	fd;

	if (!fdl)
		return ;

	while (*fdl)
	{
		fd = fdl_pop_front(fdl);
		if (fd > 0)
			close(fd);
	}
}
