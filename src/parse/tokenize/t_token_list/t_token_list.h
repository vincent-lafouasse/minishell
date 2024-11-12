/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_list.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:51:07 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 20:12:50 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_TOKEN_LIST_H
# define T_TOKEN_LIST_H

# include "../t_token.h"
# include "error/t_error.h"

typedef void			(*t_destructor)(void *);

typedef struct s_token_list
{
	t_token				token;
	struct s_token_list	*next;
	struct s_token_list	*prev;
}						t_token_list;

t_token_list	*tkl_new(t_token token);
void			tkl_push_back_link(t_token_list **tokens_p,
					t_token_list *link);
t_error			tkl_push_back(t_token_list **tokens_p, t_token token);
void			tkl_delone(t_token_list **tokens_p,
					t_destructor literal_destructor);
void			tkl_clear(t_token_list **tokens_p,
					t_destructor literal_destructor);

#endif
