#pragma once

#include "error/t_error.h"

typedef struct s_fd_list {
	int fd;
	struct s_fd_list* next;
} t_fd_list;

t_fd_list* fdl_new(int fd);

void fdl_push_back_link(t_fd_list** fdl, t_fd_list* link);
void fdl_push_front_link(t_fd_list** fdl, t_fd_list* link);
t_fd_list* fdl_pop_front_link(t_fd_list** fdl);

t_error fdl_push_back(t_fd_list** fdl, int fd);
t_error fdl_push_front(t_fd_list** fdl, int fd);
int fdl_pop_front(t_fd_list** fdl);

void fdl_clear(t_fd_list** fdl);
