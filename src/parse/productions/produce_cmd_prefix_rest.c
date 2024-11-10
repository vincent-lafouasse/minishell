#include "productions.h"

t_symbol	produce_cmd_prefix_rest(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_CMD_PREFIX_REST, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){R_PAREN, PIPE, OR_OR,
			AND_AND, WORD, EOF_TOKEN}, 6))
	{
	}
	else if (parser_matches_one_of(state, (t_token_type[]){DL_ANGLE_BRACKET,
			R_ANGLE_BRACKET, DR_ANGLE_BRACKET, L_ANGLE_BRACKET}, 4))
	{
		if (parser_produce_push(state, produce_io_redirect, symbol.production))
			parser_produce_push(state, produce_cmd_prefix_rest,
				symbol.production);
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
