/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   associativity_inversal_internals.h                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:51:30 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 19:53:00 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSOCIATIVITY_INVERSAL_INTERNALS_H
# define ASSOCIATIVITY_INVERSAL_INTERNALS_H

# include "reduction_internals.h"

typedef struct s_cond_data
{
	t_command				*commands;
	t_conditional_operator	*operators;
	size_t					connector_count;
}							t_cond_data;

t_cond_data					cond_data_allocate(size_t connector_count);
void						destroy_cond_data(t_cond_data data, size_t start);

#endif
