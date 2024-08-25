#pragma once

#include "tokenize/t_token.h"
#include "command/t_command.h"

t_command parse_command(TokenList tokens);
