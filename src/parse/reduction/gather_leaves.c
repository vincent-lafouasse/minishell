/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather_leaves.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:36 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:25:37 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tokenize/t_token_list/t_token_list.h"
#include "parse/tokenize/t_token.h"
#include "reduction_internals.h"
#include <stdlib.h>

static t_error	recurse(t_symbol *root, t_token_list **leaves_p)
{
	size_t	i;
	t_error	err;
	t_token	copy;

	i = 0;
	if (root->kind == SYM_TERMINAL)
	{
		err = token_duplicate(root->token, &copy);
		if (err != NO_ERROR)
			return (err);
		err = tkl_push_back(leaves_p, copy);
		if (err != NO_ERROR)
			free(copy.literal);
		return (err);
	}
	while (i < root->production->len)
	{
		err = recurse(&root->production->data[i], leaves_p);
		if (err != NO_ERROR)
			return (err);
		i++;
	}
	return (NO_ERROR);
}

t_error	gather_leaves(t_symbol *root, t_token_list **out)
{
	t_error	err;

	err = recurse(root, out);
	if (err != NO_ERROR)
	{
		tkl_clear(out, free);
		return (err);
	}
	return (NO_ERROR);
}
