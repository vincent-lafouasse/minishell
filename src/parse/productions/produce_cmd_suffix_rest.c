#include "productions.h"
#include "../t_symbol.h"
#include "../t_parser.h"

t_symbol	produce_cmd_suffix_rest(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(CMD_SUFFIX_REST, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	if (parser_matches_one_of(state, (t_token_type[]){DL_ANGLE_BRACKET, R_ANGLE_BRACKET, DR_ANGLE_BRACKET, L_ANGLE_BRACKET}, 4))
	{
		if (parser_produce_push(state, produce_io_redirect, symbol.production))
			parser_produce_push(state, produce_cmd_suffix_rest, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){WORD}, 1))
	{
		if (parser_accept_push(state, WORD, symbol.production))
			parser_produce_push(state, produce_cmd_suffix_rest, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){OR_OR, PIPE_TOKEN, EOF_TOKEN, R_PAREN, AND_AND}, 5))
	{
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
