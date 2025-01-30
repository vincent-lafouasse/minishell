/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exit_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:35:36 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:39:40 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

#include <stdlib.h>
#include <sys/wait.h>

int	get_exit_status(int status)
{
	if (WIFSIGNALED (status))
		return (128 + WTERMSIG (status));
	else if (WIFEXITED(status))
		return (WEXITSTATUS (status));
	else
		return (EXIT_SUCCESS);
}
