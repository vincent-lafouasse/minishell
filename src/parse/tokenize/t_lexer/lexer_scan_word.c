/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_scan_word.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:46:08 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:50:22 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/t_error.h"
#include "lexer_internals.h"
#include "libft/ctype.h"
#include "libft/ft_string.h"
#include "libft/string.h"
#include <stdlib.h>

#define SIMPLE_WORD_BREAK "\"' \t\n()<>|&"

t_error	extract_quoted_string(t_lexer *lexer, char **str_out);
t_error	extract_simple_word(t_lexer *lexer, char **str_out);
t_error	append_next_word_or_string(t_lexer *lexer, char **onto);

t_error	lexer_scan_word(t_lexer *lexer, t_token *out)
{
	char	*str_out;
	bool	must_continue;
	t_error	err;

	must_continue = true;
	str_out = ft_strdup("");
	if (str_out == NULL)
		return (E_OOM);
	while (must_continue)
	{
		err = append_next_word_or_string(lexer, &str_out);
		if (err != NO_ERROR)
		{
			free(str_out);
			return (err);
		}
		must_continue = ft_strchr(SHELL_BREAK_CHARS, lexer_peek(lexer)) == NULL;
	}
	return (fill_token((t_token){.type = WORD, .literal = str_out}, out));
}

t_error	append_next_word_or_string(t_lexer *lexer, char **onto)
{
	char	*buffer;
	char	*next;
	t_error	err;

	if (ft_strchr("\"'", lexer_peek(lexer)))
		err = extract_quoted_string(lexer, &next);
	else
		err = extract_simple_word(lexer, &next);
	if (err != NO_ERROR)
		return (err);
	buffer = ft_strjoin((const char *)*onto, next);
	if (!buffer)
	{
		free(next);
		return (E_OOM);
	}
	free(*onto);
	free(next);
	*onto = buffer;
	return (NO_ERROR);
}

t_error	extract_simple_word(t_lexer *lexer, char **str_out)
{
	size_t	start;
	size_t	sz;
	char	*out;

	start = lexer->current;
	while (ft_strchr(SIMPLE_WORD_BREAK, lexer_peek(lexer)) == NULL
		&& lexer->current < lexer->src_len)
		lexer_advance(lexer);
	sz = lexer->current - start;
	out = ft_substr(lexer->source, start, sz);
	if (out == NULL)
		return (E_OOM);
	*str_out = out;
	return (NO_ERROR);
}

t_error	extract_quoted_string(t_lexer *lexer, char **str_out)
{
	size_t	start;
	char	stop;
	size_t	sz;
	char	*out;

	start = lexer->current;
	stop = lexer_advance(lexer);
	while (lexer_peek(lexer) != stop && lexer->current < lexer->src_len)
		lexer_advance(lexer);
	if (lexer->current >= lexer->src_len)
		return (E_UNTERMINATED_QUOTE);
	lexer_advance(lexer);
	sz = lexer->current - start;
	out = ft_substr(lexer->source, start, sz);
	if (out == NULL)
		return (E_OOM);
	*str_out = out;
	return (NO_ERROR);
}
