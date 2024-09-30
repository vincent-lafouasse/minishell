#include "../t_parser.h"
#include "../t_symbol.h"
#include "productions.h"

t_symbol	produce_word_list_rest(t_parser *state)
{
	t_symbol	symbol;

	symbol = symbol_new_non_terminal(WORD_LIST_REST, 2);
	if (symbol.production == NULL)
	{
		state->err = E_OOM;
		return (symbol);
	}
	if (parser_matches_one_of(state, (t_token_type[]){OR_OR, EOF_TOKEN,
			PIPE_TOKEN, AND_AND}, 4))
	{
	}
	else if (parser_matches_one_of(state, (t_token_type[]){WORD}, 1))
	{
		if (parser_accept_push(state, WORD, symbol.production))
			parser_produce_push(state, produce_word_list_rest,
				symbol.production);
	}
	else
	{
		state->err = E_UNEXPECTED_TOKEN;
	}
	return (symbol);
}
