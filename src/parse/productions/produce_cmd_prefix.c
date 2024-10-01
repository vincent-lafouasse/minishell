#include "productions.h"
#include "../t_symbol.h"
#include "../t_parser.h"

t_symbol	produce_cmd_prefix(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(CMD_PREFIX, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	if (parser_produce_push(state, produce_io_redirect, symbol.production))
		parser_produce_push(state, produce_cmd_prefix_rest, symbol.production);
	return (symbol);
}
