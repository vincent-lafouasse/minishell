/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_string.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamar <jamar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 17:09:01 by jamar             #+#    #+#             */
/*   Updated: 2024/01/31 15:49:23 by jamar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_STRING_H
# define T_STRING_H

# include <stddef.h>
# include <stdbool.h>
# include "error/t_error.h"

# ifndef T_STRING_DEFAULT_CAPACITY
#  define T_STRING_DEFAULT_CAPACITY 64
# endif

typedef bool	t_oom;

typedef struct s_string {
	size_t	cap;
	size_t	len;
	char	data[];
}	t_string;

t_string	*string_new(void);
t_string	*string_new_with_capacity(size_t cap);

t_oom		string_push(t_string **self, char t);
t_oom		string_extend(t_string **self, const char *str);
t_oom		string_extend_n(t_string **self, const char *str, size_t n);

t_error		string_make_c_string(const t_string *self, char **out);

t_oom		string_resize(t_string **self, size_t new_cap);
t_oom		string_reserve(t_string **self, size_t additional);
t_oom		string_reserve_exact(t_string **self, size_t additional);

void		string_destroy(t_string *self);

#endif // T_STRING_H
