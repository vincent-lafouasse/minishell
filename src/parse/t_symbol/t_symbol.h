/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_symbol.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:55:48 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 20:14:00 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_SYMBOL_H
# define T_SYMBOL_H

# include "../tokenize/t_token.h"
# include <stdbool.h>
# include <stddef.h>

typedef struct s_symbol_array	t_symbol_array;
enum							e_symbol_kind :	int;

typedef struct s_symbol
{
	enum e_symbol_kind	kind;
	union
	{
		t_symbol_array	*production;
		t_token			token;
	};
}						t_symbol;

typedef enum e_symbol_kind : int
{
	SYM_TERMINAL,
	SYM_COMPLETE_CMD,
	SYM_SUBSHELL,
	SYM_CMD_PREFIX_REST,
	SYM_IO_REDIRECT,
	SYM_IO_FILE,
	SYM_PROGRAM,
	SYM_CMD_PREFIX_PRECEDES,
	SYM_IO_HERE,
	SYM_COMPLETE_CMD_REST,
	SYM_MAYBE_CMD_SUFFIX,
	SYM_CMD_SUFFIX,
	SYM_PIPELINE_REST,
	SYM_PIPELINE,
	SYM_REDIRECT_LIST,
	SYM_SIMPLE_CMD,
	SYM_HERE_END,
	SYM_FILENAME,
	SYM_CMD_PREFIX,
	SYM_COMMAND,
	SYM_CMD_SUFFIX_REST,
	SYM_REDIRECT_LIST_REST,
	SYM_SUBSHELL_PRECEDES,
}	t_symbol_kind;

typedef struct s_symbol_array
{
	size_t			len;
	size_t			cap;
	struct s_symbol	data[];
}					t_symbol_array;

t_symbol_array	*symbol_array_with_cap(size_t cap);
bool			symbol_array_push(t_symbol_array *self,
					t_symbol symbol);

t_symbol		symbol_new_non_terminal(t_symbol_kind kind,
					size_t capacity);
t_symbol		symbol_new_terminal(t_token token);

void			*symbol_clear(t_symbol symbol);

#endif
