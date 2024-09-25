#include "t_parse_tree.h"
#include "tokenize/t_token.h"

bool	parser_matches(const t_parser *parser, t_token_type ty)
{
	return (parser_peek_token(parser)->type == ty);
}

bool	parser_match_terminal(t_parser *parser, t_token_type ty)
{
	bool matches;

	matches = parser_matches(parser, ty);
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
