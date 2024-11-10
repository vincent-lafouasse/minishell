#include "productions.h"

t_symbol	produce_program(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_PROGRAM, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	parser_produce_push(state, produce_complete_command, symbol.production);
	return (symbol);
}
