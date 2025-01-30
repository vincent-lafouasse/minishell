/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_kind.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:54:39 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:54:40 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_redir_list.h"
#include <fcntl.h>
#include <unistd.h>

int	open_flags_for_redir_kind(t_redir_kind kind)
{
	if (kind == FROM_FILE)
		return (O_RDONLY);
	else if (kind == INTO_FILE)
		return (O_CREAT | O_WRONLY | O_TRUNC);
	else if (kind == APPEND_INTO_FILE)
		return (O_CREAT | O_WRONLY | O_APPEND);
	return (-1);
}

int	redirectee_fd_for_redir_kind(t_redir_kind kind)
{
	if (kind == FROM_FILE || kind == HERE_DOCUMENT)
		return (STDIN_FILENO);
	else if (kind == INTO_FILE || kind == APPEND_INTO_FILE)
		return (STDOUT_FILENO);
	return (-1);
}
