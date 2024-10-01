#pragma once

#include "error/t_error.h"
#include "tokenize/t_token_list/t_token_list.h"
#include "t_command/t_command.h"
#include "t_symbol/t_symbol.h"

t_error parse_command(t_token_list *tokens, t_symbol *out);
