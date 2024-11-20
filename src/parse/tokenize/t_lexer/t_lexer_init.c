/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_lexer_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:46:16 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:46:18 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/string.h"
#include "t_lexer.h"

t_lexer	lexer_init(const char *source)
{
	return ((t_lexer){.source = source, .src_len = ft_strlen(source)});
}
