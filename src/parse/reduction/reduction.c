#include "reduction.h"
#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

t_command	reduce_parse_tree_into_command(t_symbol *parse_tree)
{
	t_command out;

	out = reduce_complete_command(parse_tree);
	symbol_clear(*parse_tree);
	return out;
}
