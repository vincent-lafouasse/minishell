#include "productions.h"

t_symbol	produce_redirect_list_rest(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(SYM_REDIRECT_LIST_REST, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){DL_ANGLE_BRACKET,
			R_ANGLE_BRACKET, DR_ANGLE_BRACKET, L_ANGLE_BRACKET}, 4))
	{
		if (parser_produce_push(state, produce_io_redirect, symbol.production))
			parser_produce_push(state, produce_redirect_list_rest,
				symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){EOF_TOKEN, R_PAREN,
			PIPE, AND_AND, OR_OR}, 5))
	{
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
