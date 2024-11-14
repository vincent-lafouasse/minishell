#include "productions.h"

t_symbol	produce_redirect_list(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_REDIRECT_LIST, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_produce_push(state, produce_io_redirect, symbol.production))
		parser_produce_push(state, produce_redirect_list_rest,
			symbol.production);
	return (symbol);
}
