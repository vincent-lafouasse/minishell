#include "reduction.h"
#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

t_error	reduce_parse_tree_into_command(t_symbol *parse_tree, t_command *out)
{
	t_error err;

	err = reduce_complete_command(parse_tree, out);
	symbol_clear(*parse_tree);
	return err;
}
