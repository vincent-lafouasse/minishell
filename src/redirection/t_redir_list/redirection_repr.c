/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_repr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:54:41 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:54:42 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_redir_list.h"

const char	*redir_kind_repr(t_redir_kind kind)
{
	if (kind == FROM_FILE)
		return ("<");
	else if (kind == INTO_FILE)
		return (">");
	else if (kind == APPEND_INTO_FILE)
		return (">>");
	else if (kind == HERE_DOCUMENT)
		return ("<<");
	return ("Unknown redirection: ");
}
