#pragma once

#include "t_token_list/t_token_list.h"
#include "error/t_error.h"

//!  Split a string into tokens, mostly words, strings and operators
/*!
  word and string tokens own a copy of their lexeme
*/
t_error tokenize(const char *input, t_token_list** out);
