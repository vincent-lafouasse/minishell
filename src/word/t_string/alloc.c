/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 19:24:50 by jamar             #+#    #+#             */
/*   Updated: 2025/01/30 19:33:47 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "realloc.h"
#include "t_string.h"
#include <stddef.h>
#include <stdint.h>

t_oom	string_resize(t_string **self, size_t new_cap)
{
	t_string	*new_self;
	size_t		max_capacity;
	t_string	*current_self;

	current_self = *self;
	max_capacity = SIZE_MAX / sizeof(char);
	if (new_cap > max_capacity)
		return (true);
	new_self = ft_realloc(
			current_self,
			sizeof(t_string) + (current_self->len * sizeof(char)),
			sizeof(t_string) + (new_cap * sizeof(char)) \
	);
	if (new_self == NULL)
		return (true);
	new_self->cap = new_cap;
	*self = new_self;
	return (false);
}

static inline size_t	max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	return (b);
}

t_oom	string_reserve_exact(t_string **self, size_t additional)
{
	size_t		new_cap;
	t_string	*current_self;

	current_self = *self;
	if (additional <= current_self->cap - current_self->len)
		return (false);
	if (current_self->cap > (SIZE_MAX - additional))
		return (true);
	new_cap = current_self->cap + additional;
	return (string_resize(self, new_cap));
}

t_oom	string_reserve(t_string **self, size_t additional)
{
	size_t		doubled_cap;
	size_t		summed_cap;
	t_string	*current_self;

	current_self = *self;
	if (additional <= current_self->cap - current_self->len)
		return (false);
	if (current_self->cap > (SIZE_MAX / 2) || \
		current_self->cap > (SIZE_MAX - additional))
		return (true);
	doubled_cap = current_self->cap * 2;
	summed_cap = current_self->cap + additional;
	return (string_resize(self, max(doubled_cap, summed_cap)));
}
