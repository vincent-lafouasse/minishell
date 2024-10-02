#pragma once

#include "parse/t_symbol/t_symbol.h"
#include "parse/t_command/t_command.h"

t_symbol* find_symbol(t_symbol* root, t_symbol_kind kind);
t_command	reduce_complete_command(t_symbol *root);
t_command	reduce_simple_command(t_symbol *root);
t_command	reduce_command(t_symbol *root);
t_command	reduce_pipeline(t_symbol *root);
