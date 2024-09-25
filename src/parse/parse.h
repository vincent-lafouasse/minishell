#pragma once

#include "command/t_command.h"
#include "tokenize/tokenize.h"

t_command parse_command(t_token_list *tokens);
