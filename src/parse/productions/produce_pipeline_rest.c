#include "../t_parser.h"
#include "../t_symbol.h"
#include "productions.h"

t_symbol	produce_pipeline_rest(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(PIPELINE_REST, 3);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){PIPE_TOKEN}, 1))
	{
		if (parser_accept_push(state, PIPE_TOKEN, symbol.production))
			if (parser_produce_push(state, produce_command, symbol.production))
				parser_produce_push(state, produce_pipeline_rest,
					symbol.production);
	}
	else if (parser_matches_one_of(state, (t_token_type[]){OR_OR, AND_AND,
			EOF_TOKEN}, 3))
	{
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
