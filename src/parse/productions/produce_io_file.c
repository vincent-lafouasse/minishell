#include "productions.h"
#include "../t_symbol.h"
#include "../t_parser.h"

t_symbol	produce_io_file(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(IO_FILE, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	if (parser_matches_one_of(state, (t_token_type[]){DR_ANGLE_BRACKET}, 1))
	{
		if (parser_accept_push(state, DR_ANGLE_BRACKET, symbol.production))
			parser_produce_push(state, produce_filename, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){L_ANGLE_BRACKET}, 1))
	{
		if (parser_accept_push(state, L_ANGLE_BRACKET, symbol.production))
			parser_produce_push(state, produce_filename, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){R_ANGLE_BRACKET}, 1))
	{
		if (parser_accept_push(state, R_ANGLE_BRACKET, symbol.production))
			parser_produce_push(state, produce_filename, symbol.production);
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
