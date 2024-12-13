#pragma once

#include "parse/t_command/t_command.h"
#include "../execute.h"
#include <stdbool.h>

#define EX_BUILTIN_BAD_USAGE 2

bool is_builtin_command(const t_simple* simple);

t_command_result execute_echo(t_state *state, t_simple *builtin);
t_command_result execute_exit(t_state *state, t_simple *builtin);
t_command_result execute_env(t_state *state, t_simple *builtin);
t_command_result execute_pwd(t_state *state, t_simple *builtin);
t_command_result execute_cd(t_state *state, t_simple *builtin);
t_command_result execute_unset(t_state *state, t_simple *builtin);
t_command_result execute_export(t_state *state, t_simple *builtin);
