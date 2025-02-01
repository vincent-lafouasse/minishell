/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:11:50 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:11:51 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.h"
#include <stdio.h>

void	log_token(t_token token)
{
	printf("%s", token_repr(token));
	if (token.literal)
		printf("\t%s", token.literal);
}

void	log_token_list(const t_token_list *tkl)
{
	while (tkl)
	{
		printf("token:\t");
		log_token(tkl->token);
		printf("\n");
		tkl = tkl->next;
	}
}
