#include "productions.h"
#include "../t_symbol.h"
#include "../t_parser.h"

t_symbol	produce_complete_command(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(COMPLETE_COMMAND, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	if (parser_produce_push(state, produce_pipeline, symbol.production))
		parser_produce_push(state, produce_complete_command_rest, symbol.production);
	return (symbol);
}
