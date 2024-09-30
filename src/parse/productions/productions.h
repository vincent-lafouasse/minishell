#pragma once

#include "../t_parser.h"
#include "../t_symbol.h"
#pragma once

t_symbol	produce_word_list_rest(t_parser *state);
t_symbol	produce_pipeline_rest(t_parser *state);
t_symbol	produce_complete_command(t_parser *state);
t_symbol	produce_complete_command_rest(t_parser *state);
t_symbol	produce_program(t_parser *state);
t_symbol	produce_pipeline(t_parser *state);
t_symbol	produce_command(t_parser *state);
t_symbol	produce_word_list(t_parser *state);
