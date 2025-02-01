/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_syscall_related.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:07:34 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:09:01 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_error.h"
#include <stdbool.h>

bool	is_syscall_related(t_error err)
{
	return (err == E_OOM || err == E_FORK || err == E_DUP2 || err == E_EXECVE
		|| err == E_ACCESS || err == E_GETCWD || err == E_WRITE || err == E_WAIT
		|| err == E_OPEN || err == E_UNLINK || err == E_PIPE);
}
