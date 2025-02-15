/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_scan_metachar_operator.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:46:04 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:47:25 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internals.h"

static t_error	lexer_scan_redirection_operators(t_lexer *lexer, t_token *out,
					char current);

t_error	lexer_scan_metacharacter_token(t_lexer *lexer, t_token *out)
{
	char	current;

	current = lexer_advance(lexer);
	if (current == '(')
		return (fill_token((t_token){.type = L_PAREN}, out));
	if (current == ')')
		return (fill_token((t_token){.type = R_PAREN}, out));
	if (current == '|' && lexer_peek(lexer) == '|')
		return (lexer_advance(lexer), fill_token((t_token){.type = OR_OR},
			out));
	if (current == '|')
		return (fill_token((t_token){.type = PIPE}, out));
	if (current == '&' && lexer_peek(lexer) == '&')
		return (lexer_advance(lexer), fill_token((t_token){.type = AND_AND},
			out));
	if (current == '<' || current == '>')
		return (lexer_scan_redirection_operators(lexer, out, current));
	return (E_UNRECOGNIZED_TOKEN);
}

t_error	lexer_scan_redirection_operators(t_lexer *lexer, t_token *out,
		char current)
{
	if (current == '<')
	{
		if (lexer_peek(lexer) == '<')
		{
			lexer_advance(lexer);
			return (fill_token((t_token){.type = DL_ANGLE_BRACKET}, out));
		}
		else
		{
			return (fill_token((t_token){.type = L_ANGLE_BRACKET}, out));
		}
	}
	if (current == '>')
	{
		if (lexer_peek(lexer) == '>')
		{
			lexer_advance(lexer);
			return (fill_token((t_token){.type = DR_ANGLE_BRACKET}, out));
		}
		else
		{
			return (fill_token((t_token){.type = R_ANGLE_BRACKET}, out));
		}
	}
	return (E_UNREACHABLE);
}
