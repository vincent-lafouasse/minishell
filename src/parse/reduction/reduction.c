/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:51 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:25:52 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reduction.h"
#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

t_error	reduce_parse_tree_into_command(t_symbol *parse_tree, t_command *out)
{
	t_error err;

	err = reduce_complete_command(parse_tree, out);
	return err;
}
