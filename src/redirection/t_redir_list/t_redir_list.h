/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_redir_list.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:54:46 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:00:29 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_REDIR_LIST_H
# define T_REDIR_LIST_H

# include "error/t_error.h"
# include <stddef.h>

typedef void				(*t_destructor)(void *);

typedef enum e_redir_kind
{
	FROM_FILE,
	INTO_FILE,
	APPEND_INTO_FILE,
	HERE_DOCUMENT,
}							t_redir_kind;

const char					*redir_kind_repr(t_redir_kind kind);

/*
	char	*contents;     // null right after parsing
	char	*here_doc_eof; // pre-expansion
*/
typedef struct s_here_doc
{
	char					*contents;
	char					*here_doc_eof;
}							t_here_doc;

typedef struct s_redirect
{
	t_redir_kind			kind;
	union
	{
		char				*filename;
		t_here_doc			doc;
	};
}							t_redirect;

typedef struct s_redir_list
{
	struct s_redir_list		*next;
	t_redirect				redirect;
}							t_redir_list;

typedef struct s_command	t_command;
typedef struct s_state		t_state;

t_error						gather_here_documents(t_command cmd);
t_error						apply_redirections(t_state *state,
								t_redir_list *redirections);

int							open_flags_for_redir_kind(t_redir_kind kind);
int							redirectee_fd_for_redir_kind(t_redir_kind kind);

void						redirect_clear(t_redirect *redir, t_destructor del);

t_redir_list				*rdl_new(t_redirect redir);
t_redir_list				*rdl_clone(const t_redir_list *rdl);

size_t						rdl_len(const t_redir_list *rdl);

// used in reduction.c
t_error						rdl_push_back(t_redir_list **rdl, t_redirect redir);
void						rdl_push_back_link(t_redir_list **rdl,
								t_redir_list *link);
t_redir_list				*rdl_pop(t_redir_list **rdl);

void						rdl_delone(t_redir_list **rdl, t_destructor del);
void						rdl_clear(t_redir_list **rdl, t_destructor del);

char						**rdl_into_word_array(t_redir_list **rdl);

#endif