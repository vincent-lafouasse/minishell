/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 21:12:17 by poss              #+#    #+#             */
/*   Updated: 2024/04/17 19:21:01 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/t_list.h"
#include <stddef.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*out;
	t_list	*new;

	if (!lst || !f || !del)
		return (NULL);
	out = NULL;
	while (lst)
	{
		new = ft_lstnew((*f)(lst->content));
		if (!new)
			return (ft_lstclear(&out, del), NULL);
		ft_lstadd_back(&out, new);
		lst = lst->next;
	}
	return (out);
}
