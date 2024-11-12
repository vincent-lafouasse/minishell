/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internals.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:46:02 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 20:06:30 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNALS_H
# define LEXER_INTERNALS_H

# include "t_lexer.h"

# define METACHARACTERS "()<>|&"
# define WHITESPACE " \t\n"
# define SHELL_BREAK_CHARS " \t\n()<>|&"

t_error	lexer_scan_metacharacter_token(t_lexer *lexer, t_token *out);
t_error	lexer_scan_word(t_lexer *lexer, t_token *out);

char	lexer_advance(t_lexer *lexer);
char	lexer_peek(t_lexer *lexer);

t_error	fill_token(t_token token, t_token *out);

#endif
