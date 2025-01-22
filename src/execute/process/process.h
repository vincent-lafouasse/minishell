/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:48:43 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:52:58 by poss             ###   ########.fr       */
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

# include "../t_pid_list/t_pid_list.h"
# include "error/t_error.h"
# include "execute/execute.h"
# include <sys/types.h>
# include <termios.h>

t_error	fork_process(bool *is_inside_of_child, int *pid_out);

t_error	wait_for_process(t_state *state, pid_t pid, int *exit_status_out);
t_error	wait_for_pipeline(t_state *state, const t_pid_list *pids,
			int *last_exit_status_out);

// DUMMY: ensure this resets tty settings
// after killing all pids if we are interactive
t_error	kill_pipeline(t_state *state, const t_pid_list *pids);

// internals
void	report_signal_related_exit(int status);
pid_t	wait_through_signals(pid_t pid, int *status_out);
int		get_exit_status(int status);
void	reset_tty_properties(int tty_fd, struct termios *tty_properties);

#endif // PROCESS_H
