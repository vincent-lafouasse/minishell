/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_clone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:32:57 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:32:58 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/string.h"
#include "word.h"

t_error	word_clone(const char *word, char **out)
{
	char	*dup;

	dup = ft_strdup(word);
	if (!dup)
		return (E_OOM);
	*out = dup;
	return (NO_ERROR);
}
