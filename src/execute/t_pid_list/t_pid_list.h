#pragma once

#include <unistd.h>

typedef struct s_pid_list {
	pid_t pid;
	struct s_pid_list* next;
} t_pid_list;
