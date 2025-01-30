/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_redir_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:54:43 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:51:55 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_redir_list.h"
#include <stdlib.h>

void	redirect_clear(t_redirect *redir, t_destructor del)
{
	if (!del)
		return ;
	if (redir->kind == HERE_DOCUMENT)
	{
		del(redir->doc.here_doc_eof);
		del(redir->doc.contents);
	}
	else
		del(redir->filename);
	*redir = (t_redirect){0};
}

t_redir_list	*rdl_new(t_redirect redir)
{
	t_redir_list	*out;

	out = malloc(sizeof(*out));
	if (!out)
		return (NULL);
	*out = (t_redir_list){.redirect = redir, .next = NULL};
	return (out);
}

void	rdl_delone(t_redir_list **rdl, t_destructor del)
{
	t_redir_list	*buffer;

	if (!rdl || !*rdl)
		return ;
	buffer = (*rdl)->next;
	redirect_clear(&(*rdl)->redirect, del);
	free(*rdl);
	*rdl = buffer;
	return ;
}

void	rdl_clear(t_redir_list **rdl, t_destructor del)
{
	if (!rdl)
		return ;
	while (*rdl)
		rdl_delone(rdl, del);
}

size_t	rdl_len(const t_redir_list *rdl)
{
	size_t	i;

	i = 0;
	while (rdl)
	{
		rdl = rdl->next;
		i++;
	}
	return (i);
}
