#pragma once

#include "t_token.h"
#include "t_token_list/t_token_list.h"

//!  Split a string into tokens, mostly words, strings and operators
/*!
  Each token owns a copy of its lexeme (if applicable)
*/
t_token_list *tokenize(const char *input);
