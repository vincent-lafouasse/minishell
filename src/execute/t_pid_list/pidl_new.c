/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pidl_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:43:35 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:43:47 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
