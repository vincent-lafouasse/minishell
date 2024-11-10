#include "productions.h"

t_symbol	produce_io_redirect(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_IO_REDIRECT, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	if (parser_matches_one_of(state, (t_token_type[]){DR_ANGLE_BRACKET, R_ANGLE_BRACKET, L_ANGLE_BRACKET}, 3))
	{
		parser_produce_push(state, produce_io_file, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){DL_ANGLE_BRACKET}, 1))
	{
		parser_produce_push(state, produce_io_here, symbol.production);
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
