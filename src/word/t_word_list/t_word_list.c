/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_word_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:34:46 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:34:48 by poss             ###   ########.fr       */
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

void	wl_push_back_link(t_word_list **words, t_word_list *link)
{
	t_word_list	*last;

	if (words == NULL)
		return ;
	if (*words == NULL)
	{
		*words = link;
		return ;
	}
	last = *words;
	while (last->next)
		last = last->next;
	last->next = link;
	link->prev = last;
}

t_error	wl_push_back(t_word_list **words, char *contents)
{
	t_word_list	*new_last;

	new_last = wl_new(contents);
	if (new_last == NULL)
		return (E_OOM);
	wl_push_back_link(words, new_last);
	return (NO_ERROR);
}

void	wl_delone(t_word_list **words, t_destructor del)
{
	t_word_list	*next;
	t_word_list	*prev;

	if (!words || !*words)
		return ;
	next = (*words)->next;
	prev = (*words)->prev;
	if (del)
		del((*words)->contents);
	free(*words);
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;
	*words = next;
	return ;
}

void	wl_clear(t_word_list **words, t_destructor del)
{
	if (!words)
		return ;
	while (*words)
		wl_delone(words, del);
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
