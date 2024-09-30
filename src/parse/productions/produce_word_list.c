#include "../t_parser.h"
#include "../t_symbol.h"
#include "productions.h"

t_symbol	produce_word_list(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(WORD_LIST, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_accept_push(state, WORD, symbol.production))
		parser_produce_push(state, produce_word_list_rest, symbol.production);
	return (symbol);
}
