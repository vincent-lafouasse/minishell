/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:45:58 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:46:20 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internals.h"

t_error	fill_token(t_token token, t_token *out)
{
	if (!out)
		return (E_NULL_PARAM);
	*out = token;
	return (NO_ERROR);
}
