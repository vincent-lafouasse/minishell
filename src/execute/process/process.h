/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:35:47 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:36:22 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

// turns out implementing even minishell's reduced subset of job control is a
// mess without global variables!
//
// all functions try to reap all zombies and leave no child process unwaited
// for. though, all bets are off if a child process somehow remains. it is
// preferred to have no processes under you before calling `wait_for`s.

# include "../execute.h"
# include "../t_pid_list/t_pid_list.h"
# include "error/t_error.h"
# include "shell/shell.h"
# include <stdbool.h>
# include <sys/types.h>
# include <termios.h>

t_error	fork_process(bool *in_child, int *pid_out);
t_error	fork_and_push_pid(bool *in_child, t_pid_list **pidl_out);

t_error	wait_for_process(t_state *state, pid_t pid, int *exit_status_out);
t_error	wait_for_pipeline(t_state *state, const t_pid_list *pids,
			int *last_exit_status_out);

void	kill_pipeline(t_state *state, const t_pid_list *pids);
void	kill_process(t_state *state, pid_t pid);

// internals
void	report_signal_related_exit(int status);
pid_t	wait_through_signals(pid_t pid, int *status_out);
int		get_exit_status(int status);
void	reset_tty_properties(int tty_fd, struct termios *tty_properties);

#endif // PROCESS_H
