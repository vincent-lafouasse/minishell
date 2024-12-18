#ifndef PROCESS_H
# define PROCESS_H

// turns out implementing even minishell's reduced subset of job control is a
// mess without global variables!
//
// all functions try to reap all zombies and leave no child process unwaited
// for. though, all bets are off if a child process somehow remains. it is
// preferred to have no processes under you before calling `wait_for`s.

#include <sys/types.h>

#include "../t_pid_list/t_pid_list.h"
#include "error/t_error.h"

t_error fork_process(bool *is_inside_of_child, int *pid_out);

// BAD(wait_for): a child process may change tty options (vim for example), and
// if it doesn't exit properly (say for example SIGKILL'd), then the tty options
// will never be reset to what they used to be. try: `vim` then in another
// terminal `pkill -KILL vim` in both minishell and bash
t_error wait_for_process(pid_t pid, int *exit_status_out);
t_error wait_for_pipeline(t_pid_list *pids, int *last_exit_status_out);

t_error kill_pipeline(t_pid_list *pids);

// internals
void report_signal_related_exit(int status);
pid_t wait_through_signals(pid_t pid, int *status_out);
int get_exit_status(int status);

#endif // PROCESS_H
