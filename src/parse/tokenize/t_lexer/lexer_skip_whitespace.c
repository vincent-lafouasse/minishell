/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_skip_whitespace.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:46:11 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:46:24 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internals.h"
#include "libft/string.h"

void	lexer_skip_whitespace(t_lexer *lexer)
{
	while (lexer->current < lexer->src_len && ft_strchr(WHITESPACE,
			lexer_peek(lexer)) != NULL)
		lexer_advance(lexer);
}
