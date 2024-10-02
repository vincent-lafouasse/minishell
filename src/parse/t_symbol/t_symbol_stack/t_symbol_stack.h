#pragma once

#include "parse/t_symbol/t_symbol.h"

typedef struct s_symbol_stack
{
	t_symbol				*sym;
	struct s_symbol_stack	*next;
}							t_symbol_stack;

t_symbol_stack				*ss_new(t_symbol *sym);
void						ss_push_link(t_symbol_stack **stack_p,
								t_symbol_stack *node);
t_symbol_stack				*ss_pop_link(t_symbol_stack **stack_p);

// interface principale
bool						ss_push(t_symbol_stack **stack_p, t_symbol *sym);
t_symbol					*ss_pop(t_symbol_stack **stack_p);

bool ss_contains(const t_symbol_stack* stack, t_symbol* sym);

void *ss_clear(t_symbol_stack **stack_p);
