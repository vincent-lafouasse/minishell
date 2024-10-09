#include "parse.h"
#include "parse/t_command/t_command.h"
#include "productions/productions.h"
#include "tokenize/tokenize.h"
#include "tokenize/t_token.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "t_symbol/t_symbol.h"
#include "reduction/reduction.h"

t_error parse_command(const t_token_list *tokens, t_symbol *out)
{
    t_parser state;

    state = parser_new(tokens);
    *out = produce_complete_command(&state);
    if (state.err != NO_ERROR)
    {
        symbol_clear(*out);
        return state.err;
    }
    if (parser_peek(&state)->type != EOF_TOKEN)
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
    tkl_clear(&tokens);
    err = reduce_parse_tree_into_command(&parse_tree, out);
    if (err != NO_ERROR)
    {
        symbol_clear(parse_tree);
        command_destroy(*out);
        return err;
    }
    symbol_clear(parse_tree);
    return (NO_ERROR);
}
