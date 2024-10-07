#include "t_symbol_stack.h"
#include <stdlib.h>

bool ss_contains(const t_symbol_stack* stack, t_symbol* sym)
{
	while (stack)
	{
		if (stack->sym == sym)
			return true;
		stack = stack->next;
	}
	return false;
}

t_symbol_stack	*ss_new(t_symbol *sym)
{
	t_symbol_stack	*out;

	out = malloc(sizeof(*out));
	if (out == NULL)
		return (NULL);
	*out = (t_symbol_stack){.sym = sym, .next = NULL};
	return (out);
}

void	ss_push_link(t_symbol_stack **stack_p, t_symbol_stack *node)
{
	t_symbol_stack	*old_head;

	if (*stack_p == NULL)
	{
		*stack_p = node;
		return ;
	}
	old_head = *stack_p;
	*stack_p = node;
	node->next = old_head;
}

t_symbol_stack	*ss_pop_link(t_symbol_stack **stack_p)
{
	t_symbol_stack	*out;

	if (*stack_p == NULL)
		return (NULL);
	out = *stack_p;
	*stack_p = (*stack_p)->next;
	out->next = NULL;
	return (out);
}

bool	ss_push(t_symbol_stack **stack_p, t_symbol *sym)
{
	t_symbol_stack	*new_head;

	new_head = ss_new(sym);
	if (new_head == NULL)
		return (false);
	ss_push_link(stack_p, new_head);
	return (true);
}

t_symbol	*ss_pop(t_symbol_stack **stack_p)
{
	t_symbol_stack	*head;
	t_symbol		*out;

	head = ss_pop_link(stack_p);
	if (head == NULL)
		return (NULL);
	out = head->sym;
	free(head);
	return (out);
}
