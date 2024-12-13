/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_advance_peek.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:46:00 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:46:22 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_lexer.h"

char	lexer_advance(t_lexer *lexer)
{
	if (lexer->current < lexer->src_len)
		return (lexer->source[lexer->current++]);
	else
		return ('\0');
}

char	lexer_peek(t_lexer *lexer)
{
	if (lexer->current <= lexer->src_len)
		return (lexer->source[lexer->current]);
	else
		return ('\0');
}
