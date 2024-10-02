#pragma once

#include "parse/t_symbol/t_symbol.h"
#include "parse/t_command/t_command.h"

t_symbol* find_symbol(t_symbol* root, t_symbol_kind kind);
t_command reduce_parse_tree_into_command(t_symbol *parse_tree);
