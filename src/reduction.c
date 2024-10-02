#include "parse/t_symbol.h"
#include "parse/t_command/t_command.h"
#include <stdlib.h>

typedef struct s_symbol_stack
{
	t_symbol* sym;
	struct s_symbol_stack* next;
} t_symbol_stack;

t_symbol_stack* ss_new(t_symbol* sym)
{
	t_symbol_stack* out;

	out = malloc(sizeof(*out));
	if (!out)
		return NULL;
	*out = (t_symbol_stack){.sym = sym, .next = NULL};
	return out;
}

void ss_push_link(t_symbol_stack** stack_p, t_symbol_stack* node)
{
	if (*stack_p == NULL)
	{
		*stack_p = node;
		return;
	}

	t_symbol_stack* last = *stack_p;
	
	while (last->next)
		last = last->next;

	last->next = node;
}

t_symbol_stack* ss_pop_link(t_symbol_stack** stack_p)
{
	if (*stack_p == NULL)
		return NULL;

	t_symbol_stack* out = *stack_p;
	*stack_p = (*stack_p)->next;
	out->next = NULL;
	return out;
}


t_command reduce_simple_command(t_symbol* root);
