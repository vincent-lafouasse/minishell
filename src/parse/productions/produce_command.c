#include "../t_parser.h"
#include "../t_symbol.h"
#include "productions.h"

t_symbol	produce_command(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(COMMAND, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	parser_produce_push(state, produce_word_list, symbol.production);
	return (symbol);
}
