#pragma once

#include "../t_symbol/t_symbol.h"
#include "../t_command/t_command.h"
#include "error/t_error.h"

t_error	reduce_parse_tree_into_command(t_symbol *parse_tree, t_command *out);
