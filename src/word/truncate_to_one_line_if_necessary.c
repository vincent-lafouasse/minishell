/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   truncate_to_one_line_if_necessary.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:54:24 by jamar             #+#    #+#             */
/*   Updated: 2025/01/31 22:54:25 by jamar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/string.h"

void	truncate_to_one_line_if_necessary(char *str)
{
	char	*line_break;

	line_break = ft_strchr(str, '\n');
	if (line_break)
		*line_break = '\0';
}
