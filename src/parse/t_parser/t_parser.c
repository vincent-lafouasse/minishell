#include "t_parser.h"
#include <stdbool.h>
#include <stddef.h>

t_parser parser_new(const t_token_list *tokens)
{
    return ((t_parser){.current = tokens, .err = NO_ERROR});
}

bool parser_matches(const t_parser *parser, t_token_type type)
{
    return (parser_peek(parser)->type == type);
}

bool parser_matches_one_of(const t_parser *parser, const t_token_type types[],
                           size_t type_count)
{
    size_t i;

    i = 0;
    while (i < type_count)
    {
        if (parser_matches(parser, types[i]))
            return (true);
        i++;
    }
    return (false);
}

__attribute__((warn_unused_result)) bool
parser_produce_push(t_parser *parser, t_symbol (*prod)(t_parser *),
                    t_symbol_array *out)
{
    t_symbol symbol;

    symbol = prod(parser);
    symbol_array_push(out, symbol);
    return (parser->err == NO_ERROR);
}

__attribute__((warn_unused_result)) bool
parser_accept_push(t_parser *parser, t_token_type type, t_symbol_array *out)
{
    bool matches;
    t_symbol symbol;

    matches = parser_matches(parser, type);
    if (!matches)
	{
		parser->err = E_UNEXPECTED_TOKEN;
        return false;
	}
    symbol = symbol_new_terminal(parser_advance(parser));
    symbol_array_push(out, symbol);
    return true;
}

const t_token *parser_peek(const t_parser *parser)
{
    return (&parser->current->token);
}

t_token parser_advance(t_parser *parser)
{
    t_token token;

    token = parser->current->token;
    if (token.type != EOF_TOKEN)
        parser->current = parser->current->next;
    // TODO: clear node either here or at the top of parse
    return (token);
}
