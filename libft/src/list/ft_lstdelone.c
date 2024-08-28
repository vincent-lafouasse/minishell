/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:57:32 by poss              #+#    #+#             */
/*   Updated: 2024/04/19 17:13:50 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/t_list.h"
#include <stdlib.h>

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst == NULL)
		return ;
	if (del)
		(*del)(lst->content);
	free(lst);
}
