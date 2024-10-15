#include "t_pid_list.h"

#include <stdlib.h>

t_pid_list* pidl_new(pid_t pid)
{
	t_pid_list* out = malloc(sizeof(*out));

	if (out == NULL)
		return NULL;
	*out = (t_pid_list){.pid = pid, .next = NULL};
	return out;
}

t_pid_list *pidl_last(t_pid_list* pidl)
{
	if (pidl == NULL)
		return NULL;

	t_pid_list* last = pidl;
	while (last->next)
		last = last->next;
	return (last);
}

void pidl_push_back_link(t_pid_list** pidl, t_pid_list* link)
{
	if (pidl == NULL)
		return ;

	t_pid_list* last = pidl_last(*pidl);

	if (last == NULL)
		*pidl = link;
	else
		last->next = link;
}

void pidl_push_front_link(t_pid_list** pidl, t_pid_list* link)
{
	if (pidl == NULL)
		return ;

	if (*pidl == NULL)
	{
		*pidl = link;
		return ;
	}

	t_pid_list* head = *pidl;
	link->next = head;
	*pidl = link;
}

t_pid_list* pidl_pop_front_link(t_pid_list** pidl)
{
	if (pidl == NULL || *pidl == NULL)
		return NULL;

	t_pid_list* out = *pidl;
	t_pid_list* new_first = out->next;
	out->next = NULL;
	*pidl = new_first;
	return out;
}

t_error pidl_push_back(t_pid_list** pidl, pid_t pid)
{
	t_pid_list* new_last = pidl_new(pid);

	if (new_last == NULL)
		return E_OOM;
	pidl_push_back_link(pidl, new_last);
	return NO_ERROR;
}

t_error pidl_push_front(t_pid_list** pidl, pid_t pid)
{
	t_pid_list* new_head = pidl_new(pid);

	if (new_head == NULL)
		return E_OOM;
	pidl_push_front_link(pidl, new_head);
	return NO_ERROR;
}

pid_t pidl_pop_front(t_pid_list** pidl)
{
	t_pid_list* front = pidl_pop_front_link(pidl);

	if (front == NULL)
		return 0;

	pid_t out = front->pid;
	free(front);
	return out;
}

void pidl_clear(t_pid_list** pidl) // bad dummy
{
	(void)pidl;
}
