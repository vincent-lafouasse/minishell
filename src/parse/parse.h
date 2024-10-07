#pragma once

#include "error/t_error.h"
#include "t_command/t_command.h"

t_error parse(const char *input, t_command *out);
