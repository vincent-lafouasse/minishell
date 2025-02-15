/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:52:29 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:52:30 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/string.h"
#include "t_token.h"

t_error	token_duplicate(t_token token, t_token *out)
{
	char	*literal_copy;

	literal_copy = NULL;
	if (token.literal != NULL)
	{
		literal_copy = ft_strdup(token.literal);
		if (!literal_copy)
			return (E_OOM);
	}
	*out = token;
	out->literal = literal_copy;
	return (NO_ERROR);
}
