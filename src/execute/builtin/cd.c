/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:29:55 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:29:56 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"
#include "builtin.h"
#include "libft/string.h"
#include "word/t_word_list/t_word_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USAGE_MESSAGE "usage: cd relative_or_absolute_path\n"

t_command_result	execute_cd(t_state *state, t_simple *builtin)
{
	t_word_list	*arguments;

	(void)state;
	arguments = builtin->words->next;
	if (!arguments || arguments->next != NULL)
	{
		write(STDERR_FILENO, USAGE_MESSAGE, ft_strlen(USAGE_MESSAGE));
		return ((t_command_result){.error = NO_ERROR,
			.status_code = EX_BADUSAGE});
	}
	if (chdir(arguments->contents) < 0)
	{
		perror("minishell: cd");
		return ((t_command_result){.error = NO_ERROR,
			.status_code = EXIT_FAILURE});
	}
	return ((t_command_result){.error = NO_ERROR, .status_code = 0});
}
