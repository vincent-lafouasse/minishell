#include "log/log.h"
#include "parse.h"
#include "t_parser.h"
#include "t_symbol.h"

t_symbol produce_complete_command(t_parser *);

/* t_error parse(const t_token_list *tokens) */
t_error parse_command(t_token_list *tokens, t_symbol *out)
{
    t_parser state;

    state = parser_new(tokens);
    *out = produce_complete_command(&state);
    if (state.err != NO_ERROR)
    {
        symbol_clear(*out);
        return state.err;
    }
    if (parser_peek_token(&state)->type != EOF_TOKEN)
    {
        symbol_clear(*out);
        return E_UNEXPECTED_TOKEN;
    }
    tree_to_json(out);
    return NO_ERROR;
}
