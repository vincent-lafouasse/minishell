/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_internals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:53:25 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 16:54:18 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export_internals.h"
#include "libft/ctype.h"
#include "libft/string.h"
#include <unistd.h>

#define NOT_A_VALID_ID "minishell: export: not a valid identifier\n"

void	report_invalid_identifier(char *identifier_name)
{
	(void)identifier_name;
	write(STDERR_FILENO, NOT_A_VALID_ID, ft_strlen(NOT_A_VALID_ID));
}

// returns 0 if assignment is malformated in any way
size_t	name_len(char *assignment)
{
	size_t	i;

	if (!ft_isalpha(*assignment) && *assignment != '_')
		return (0);
	i = 1;
	while (assignment[i])
	{
		if (assignment[i] == '=')
			return (i);
		if (assignment[i] == '+' && assignment[i + 1] == '=')
			return (i);
		if (!ft_isalnum(assignment[i]) && assignment[i] != '_')
			return (0);
		i++;
	}
	return (i);
}
