/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 19:23:12 by jamar             #+#    #+#             */
/*   Updated: 2024/01/30 20:40:30 by jamar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "realloc.h"
#include "t_string.h"
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

t_string	*string_new(void)
{
	return (string_new_with_capacity(T_STRING_DEFAULT_CAPACITY));
}

t_string	*string_new_with_capacity(size_t cap)
{
	t_string	*this;

	this = malloc(sizeof(*this) + sizeof(char) * cap);
	if (!this)
		return (NULL);
	*this = (t_string){.cap = cap, .len = 0};
	return (this);
}
