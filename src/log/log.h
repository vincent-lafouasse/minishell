#pragma once

#include "error/t_error.h"
#include "tokenize/tokenize.h"
#include "parse/t_symbol/t_symbol.h"
#include "word/t_word_list/t_word_list.h"
#include "redirection/t_redir_list/t_redir_list.h"

void log_token(t_token token);
void log_token_list(const t_token_list *tkl);
void log_error(t_error err);
void tree_to_json(const t_symbol *root);
void log_word_list(const t_word_list *words);
void log_redir_list(const t_redir_list *rdl);
