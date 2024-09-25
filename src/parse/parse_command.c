#include <stdio.h>
#include "parse.h"
#include "t_parse_tree/t_parse_tree.h"
#include "t_parse_tree/productions.h"
#include "tokenize/t_token.h"

t_command parse_command(t_token_list *tokens)
{
	t_parser	parser;
	t_parser_error	err;

	parser = parser_new(tokens);
	err = produce_start(&parser);
	if (err.type != NO_PARSE_ERROR)
		printf("encountered parse error near %s\n", token_repr(*(err.token)));
	(void)tokens;
	return (t_command){};
}

t_parser_error	produce_start(t_parser *state)
{
	if (parse_term(state).type == NO_PARSE_ERROR)
	{
		if (parser_matches(state, WORD))
		{
			/* token = */ parser_advance_token(state);
			if (parser_matches(state, EOF_TOKEN))
				return parser_ok();
		}
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	parse_term(t_parser *state)
{
	t_parser_error	err;
	/*t_token	*token;*/

	err = parser_ok();
	if (parser_matches(state, WORD))
	{
		/* token = */ parser_advance_token(state);
		err = parse_term(state);
	}
	else if (parser_matches(state, PIPE_TOKEN))
		parser_advance_token(state);
	else
		err = parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state));
	return (err);
}
}
