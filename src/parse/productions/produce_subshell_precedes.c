#include "productions.h"
#include "../t_symbol.h"
#include "../t_parser.h"

t_symbol	produce_subshell_precedes(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SUBSHELL_PRECEDES, 1);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return symbol;
	}
	if (parser_matches_one_of(state, (t_token_type[]){DR_ANGLE_BRACKET, DL_ANGLE_BRACKET, L_ANGLE_BRACKET, R_ANGLE_BRACKET}, 4))
	{
		parser_produce_push(state, produce_redirect_list, symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){PIPE_TOKEN, EOF_TOKEN, R_PAREN, OR_OR, AND_AND}, 5))
	{
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
