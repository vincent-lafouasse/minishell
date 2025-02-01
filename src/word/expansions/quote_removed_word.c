/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removed_word.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:45:53 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:45:55 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./expand.h"
#include "./expand_internals.h"
#include "error/t_error.h"

t_error	quote_removed_word(const char *word, char **out)
{
	t_error				err;
	t_word_quotes_list	*parts;

	err = wql_parse(word, &parts);
	if (err != NO_ERROR)
		return (err);
	wql_remove_outer_quotes(parts);
	err = wql_make_joined(parts, out);
	wql_clear(&parts);
	return (err);
}
