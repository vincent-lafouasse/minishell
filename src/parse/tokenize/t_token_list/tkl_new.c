/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkl_new.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:51:17 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:51:18 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_token_list.h"
#include <stdlib.h>

t_token_list	*tkl_new(t_token token)
{
	t_token_list	*out;

	out = malloc(sizeof(*out));
	if (!out)
		return (NULL);
	*out = (t_token_list){.token = token};
	return (out);
}
