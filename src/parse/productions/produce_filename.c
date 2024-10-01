#include "productions.h"
#include "../t_symbol.h"
#include "../t_parser.h"

t_symbol	produce_filename(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(FILENAME, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	parser_accept_push(state, WORD, symbol.production);
	return (symbol);
}
