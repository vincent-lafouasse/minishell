/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_pid_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:43:02 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:43:15 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_PID_LIST_H
#define T_PID_LIST_H

#include <unistd.h>

#include "error/t_error.h"

typedef struct s_pid_list {
	pid_t pid;
	struct s_pid_list* next;
} t_pid_list;

t_pid_list* pidl_new(pid_t pid);

t_pid_list *pidl_last(const t_pid_list* pidl);

void pidl_push_back_link(t_pid_list** pidl, t_pid_list* link);
void pidl_push_front_link(t_pid_list** pidl, t_pid_list* link);
t_pid_list* pidl_pop_front_link(t_pid_list** pidl);

t_error pidl_push_back(t_pid_list** pidl, pid_t pid);
t_error pidl_push_front(t_pid_list** pidl, pid_t pid);
pid_t pidl_pop_front(t_pid_list** pidl);

void pidl_clear(t_pid_list** pidl);

#endif
