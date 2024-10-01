#pragma once

#include "parse/t_symbol.h"
#include "tokenize/t_token_list/t_token_list.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct s_parser t_parser;
struct s_parser
{
    const t_token_list *_head;
    const t_token_list *current_token;
    t_error err;
};


typedef t_symbol (*t_production)(t_parser *);

t_parser parser_new(const t_token_list *tokens);
bool parser_matches(const t_parser *parser, t_token_type ty);
bool parser_matches_one_of(const t_parser *parser, const t_token_type types[],
                           size_t type_count);
bool parser_accept_push(t_parser *parser, t_token_type type,
                        t_symbol_array *out);
bool parser_produce_push(t_parser *parser, t_production prod,
                         t_symbol_array *out);
const t_token *parser_peek_token(const t_parser *parser);
t_token parser_advance_token(t_parser *parser);
