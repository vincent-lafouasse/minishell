#include "parse.h"
#include "productions/productions.h"
#include "tokenize/tokenize.h"
#include "tokenize/t_token.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "t_symbol/t_symbol.h"
#include "reduction/reduction.h"

#include "log/log.h" // todo remove

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
    return NO_ERROR;
}

t_error parse(const char *input, t_command *out)
{
    t_token_list *tokens;
    t_symbol parse_tree;
    t_error err;

    err = tokenize(input, &tokens);
    if (err != NO_ERROR)
        return err;
    err = parse_command(tokens, &parse_tree);
    if (err != NO_ERROR)
    {
        tkl_clear(&tokens);
        return err;
    }
    *out = reduce_parse_tree_into_command(&parse_tree);
    return (NO_ERROR);
}
