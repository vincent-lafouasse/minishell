#pragma once

#include "parse/t_command/t_command.h"
#include <stdbool.h>

#define EX_BUILTIN_BAD_USAGE 2

bool is_builtin_command(const t_simple* simple);
