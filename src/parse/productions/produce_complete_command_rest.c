#include "../t_parser.h"
#include "../t_symbol.h"
#include "productions.h"

t_symbol	produce_complete_command_rest(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(COMPLETE_COMMAND_REST, 3);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){AND_AND}, 1))
	{
		if (parser_accept_push(state, AND_AND, symbol.production))
			if (parser_produce_push(state, produce_pipeline, symbol.production))
				parser_produce_push(state, produce_complete_command_rest,
					symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){OR_OR}, 1))
	{
		if (parser_accept_push(state, OR_OR, symbol.production))
			if (parser_produce_push(state, produce_pipeline, symbol.production))
				parser_produce_push(state, produce_complete_command_rest,
					symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){EOF_TOKEN}, 1))
	{
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
