/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:52:31 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 20:13:20 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_TOKEN_H
# define T_TOKEN_H

# include "error/t_error.h"

typedef enum e_token_type
{
	WORD,
	PIPE,
	L_ANGLE_BRACKET,
	DL_ANGLE_BRACKET,
	R_ANGLE_BRACKET,
	DR_ANGLE_BRACKET,
	L_PAREN,
	R_PAREN,
	AND_AND,
	OR_OR,
	EOF_TOKEN,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*literal;
}					t_token;

t_error		token_duplicate(t_token token, t_token *out);

const char	*token_repr(t_token token);

#endif
