/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_word_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:34:46 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:37:30 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/stdlib.h"
#include "t_word_list.h"
#include <stdlib.h>

t_word_list	*wl_new(char *contents)
{
	t_word_list	*out;

	out = malloc(sizeof(*out));
	if (out == NULL)
		return (NULL);
	*out = (t_word_list){.contents = contents, .next = NULL, .prev = NULL};
	return (out);
}

size_t	wl_count(const t_word_list *words)
{
	size_t	i;

	i = 0;
	while (words)
	{
		words = words->next;
		i++;
	}
	return (i);
}

char	**wl_into_word_array(t_word_list **words)
{
	size_t	word_count;
	char	**out;
	size_t	i;

	word_count = wl_count(*words);
	out = ft_calloc(word_count + 1, sizeof(*out));
	if (!out)
		return (NULL);
	i = 0;
	while (*words)
	{
		out[i] = (*words)->contents;
		wl_delone(words, NULL);
		i++;
	}
	return (out);
}
