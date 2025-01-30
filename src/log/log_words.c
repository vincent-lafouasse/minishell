/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_words.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:11:52 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:11:55 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.h"
#include <stdio.h>

void	log_word_list(const t_word_list *words)
{
	while (words)
	{
		printf(" > %s", words->contents);
		printf("\n");
		words = words->next;
	}
}

void	log_redir_list(const t_redir_list *rdl)
{
	char	*word;

	while (rdl)
	{
		printf(" > %s", redir_kind_repr(rdl->redirect.kind));
		if (rdl->redirect.kind == HERE_DOCUMENT)
			word = rdl->redirect.doc.here_doc_eof;
		else
			word = rdl->redirect.filename;
		printf("%s", word);
		printf("\n");
		rdl = rdl->next;
	}
}
