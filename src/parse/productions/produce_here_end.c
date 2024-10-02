#include "productions.h"

t_symbol	produce_here_end(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(HERE_END, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	parser_accept_push(state, WORD, symbol.production);
	return (symbol);
}
