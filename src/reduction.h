#pragma once

#include "parse/t_symbol/t_symbol.h"
#include "parse/t_command/t_command.h"

t_symbol* find_simple_command(t_symbol* root);
t_command	reduce_simple_command(t_symbol *root);
