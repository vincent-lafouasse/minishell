#include "productions.h"
#include "../t_symbol.h"
#include "../t_parser.h"

t_symbol	produce_cmd_prefix_precedes(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(CMD_PREFIX_PRECEDES, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	if (parser_matches_one_of(state, (t_token_type[]){OR_OR, PIPE, AND_AND, R_PAREN, EOF_TOKEN}, 5))
	{
	}
	else if (parser_matches_one_of(state, (t_token_type[]){WORD}, 1))
	{
		if (parser_accept_push(state, WORD, symbol.production))
			parser_produce_push(state, produce_maybe_cmd_suffix, symbol.production);
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
