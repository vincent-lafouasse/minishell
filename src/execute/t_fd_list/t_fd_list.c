#include "t_fd_list.h"

#include <stdlib.h>

t_fd_list* fdl_new(int fd)
{
	t_fd_list* out = malloc(sizeof(*out));

	if (out == NULL)
		return NULL;
	*out = (t_fd_list){.fd = fd, .next = NULL};
	return out;
}

void fdl_push_back_link(t_fd_list** fdl, t_fd_list* link)
{
	if (fdl == NULL)
		return ;

	if (*fdl == NULL)
	{
		*fdl = link;
		return ;
	}

	t_fd_list* last = *fdl;
	while (last->next)
		last = last->next;
	last->next = link;
}

void fdl_push_front_link(t_fd_list** fdl, t_fd_list* link)
{
	if (fdl == NULL)
		return ;

	if (*fdl == NULL)
	{
		*fdl = link;
		return ;
	}

	t_fd_list* head = *fdl;
	link->next = head;
	*fdl = link;
}

t_fd_list* fdl_pop_front_link(t_fd_list** fdl)
{
	if (fdl == NULL || *fdl == NULL)
		return NULL;

	t_fd_list* out = *fdl;
	t_fd_list* new_first = out->next;
	out->next = NULL;
	*fdl = new_first;
	return out;
}

t_error fdl_push_back(t_fd_list** fdl, int fd)
{
	t_fd_list* new_last = fdl_new(fd);

	if (new_last == NULL)
		return E_OOM;
	fdl_push_back_link(fdl, new_last);
	return NO_ERROR;
}

t_error fdl_push_front(t_fd_list** fdl, int fd)
{
	t_fd_list* new_head = fdl_new(fd);

	if (new_head == NULL)
		return E_OOM;
	fdl_push_front_link(fdl, new_head);
	return NO_ERROR;
}

int fdl_pop_front(t_fd_list** fdl)
{
	t_fd_list* front = fdl_pop_front_link(fdl);

	if (front == NULL)
		return 0;

	int out = front->fd;
	free(front);
	return out;
}

void fdl_clear(t_fd_list** fdl) // bad dummy
{
	(void)fdl;
}
