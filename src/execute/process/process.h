#ifndef PROCESS_H
# define PROCESS_H

#include <sys/types.h>

#include "../t_pid_list/t_pid_list.h"
#include "error/t_error.h"

t_error fork_process(bool *is_inside_of_child, int *pid_out);

t_error wait_for_process(pid_t pid, int *exit_status_out);
t_error wait_for_pipeline(t_pid_list *pids, int *last_exit_status_out);

// internals
void report_signal_related_exit(int status);
pid_t wait_through_signals(pid_t pid, int *status_out);
int get_exit_status(int status);

#endif // PROCESS_H
