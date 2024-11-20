/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkl_clear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:51:11 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:51:12 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_token_list.h"

void	tkl_clear(t_token_list **tokens_p, t_destructor literal_destructor)
{
	if (!tokens_p)
		return ;
	while (*tokens_p)
		tkl_delone(tokens_p, literal_destructor);
}
