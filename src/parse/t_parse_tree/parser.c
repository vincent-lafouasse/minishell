#include <stddef.h>
#include "t_parse_tree.h"
#include "tokenize/t_token.h"

bool	parser_matches(const t_parser *parser, t_token_type type)
{
	return (parser_peek_token(parser)->type == type);
}

bool	parser_matches_one_of(const t_parser *parser, const t_token_type types[], size_t type_count)
{
	size_t	i;

	i = 0;
	while (i < type_count)
	{
		if (parser_matches(parser, types[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	parser_match_terminal(t_parser *parser, t_token_type type)
{
	bool matches;

	matches = parser_matches(parser, type);
	if (matches)
		parser_advance_token(parser);
	return (matches);
}

const t_token	*parser_peek_token(const t_parser *parser)
{
	return (&parser->current_token->token);
}

t_token	*parser_advance_token(t_parser *parser)
{
	t_token	*token;

	token = &parser->current_token->token;
	if (token->type != EOF_TOKEN)
		parser->current_token = parser->current_token->next;
	return (token);
}
