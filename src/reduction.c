#include "parse/t_symbol.h"
#include "parse/t_command/t_command.h"

typedef struct s_symbol_stack
{
	t_symbol* sym;
	struct s_symbol_stack* next;
} t_symbol_stack;

t_command reduce_simple_command(t_symbol* root);
