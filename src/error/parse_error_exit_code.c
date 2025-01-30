/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error_exit_code.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:07:40 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:07:42 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_error.h"
#include <stdlib.h>

char	parse_error_exit_code(t_error err)
{
	if (err == E_UNEXPECTED_TOKEN || err == E_UNRECOGNIZED_TOKEN)
		return (2);
	return (EXIT_FAILURE);
}
