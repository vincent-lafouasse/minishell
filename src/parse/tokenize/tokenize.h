#pragma once

#include "t_token.h"
#include "t_token_list/t_token_list.h"

//!  Split a string into tokens, mostly words, strings and operators
/*!
  word and string tokens own a copy of their lexeme
*/
t_token_list *tokenize(const char *input);
