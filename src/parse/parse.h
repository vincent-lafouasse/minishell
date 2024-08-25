#pragma once

#include "command/t_command.h"
#include "tokenize/tokenize.h"

t_command parse_command(TokenList tokens);
