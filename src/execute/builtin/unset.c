/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:30:05 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 17:30:07 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execute.h"
#include "builtin.h"
#include "execute/t_env/t_env.h"
#include "libft/ctype.h"
#include "libft/string.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

static void	report_invalid_identifier(char *identifier_name)
{
	const char	*message = "minishell: unset: invalid identifier\n";

	(void)identifier_name;
	write(STDERR_FILENO, message, ft_strlen(message));
}

static bool	is_valid_identifier(char *str)
{
	size_t	i;

	i = 0;
	if (!*str || ft_isdigit(*str))
		return (false);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

t_command_result	execute_unset(t_state *state, t_simple *builtin)
{
	t_word_list	*var;
	t_env		*entry;
	bool		got_invalid_identifier;

	(void)state;
	var = builtin->words->next;
	got_invalid_identifier = false;
	while (var)
	{
		if (!is_valid_identifier(var->contents))
		{
			report_invalid_identifier(var->contents);
			got_invalid_identifier = true;
			var = var->next;
			continue ;
		}
		entry = env_remove(&state->env, var->contents);
		if (entry)
			env_destroy(&entry);
		var = var->next;
	}
	if (got_invalid_identifier)
		return ((t_command_result){.error = NO_ERROR,
			.status_code = EXIT_FAILURE});
	return ((t_command_result){.error = NO_ERROR, .status_code = 0});
}
