/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   properties.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:56 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:57:57 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_command.h"
#include <stdbool.h>
#include <stddef.h>

bool	command_is_initialized(t_command command)
{
	return (command.any != NULL);
}
