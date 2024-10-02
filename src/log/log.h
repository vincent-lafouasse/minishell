#pragma once

#include "error/t_error.h"
#include "tokenize/tokenize.h"
#include "parse/t_symbol/t_symbol.h"

void log_token(t_token token);
void log_token_list(const t_token_list *tkl);
void log_error(t_error err);
void tree_to_json(const t_symbol *root);
