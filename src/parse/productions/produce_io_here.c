#include "productions.h"

t_symbol	produce_io_here(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(IO_HERE, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	if (parser_accept_push(state, DL_ANGLE_BRACKET, symbol.production))
		parser_produce_push(state, produce_here_end, symbol.production);
	return (symbol);
}
