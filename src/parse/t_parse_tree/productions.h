#ifndef PRODUCTIONS_H
# define PRODUCTIONS_H

# include "t_parse_tree.h"

t_parser_error	produce_start(t_parser *state);
t_parser_error	produce_complete_command(t_parser *state);
t_parser_error	produce_complete_command_rest(t_parser *state);
t_parser_error	produce_pipeline(t_parser *state);
t_parser_error	produce_pipeline_rest(t_parser *state);
t_parser_error	produce_command(t_parser *state);
t_parser_error	produce_subshell_precedes(t_parser *state);
t_parser_error	produce_subshell(t_parser *state);
t_parser_error	produce_simple_command(t_parser *state);
t_parser_error	produce_maybe_cmd_suffix(t_parser *state);
t_parser_error	produce_cmd_prefix_precedes(t_parser *state);
t_parser_error	produce_cmd_prefix(t_parser *state);
t_parser_error	produce_cmd_prefix_rest(t_parser *state);
t_parser_error	produce_cmd_suffix(t_parser *state);
t_parser_error	produce_cmd_suffix_rest(t_parser *state);
t_parser_error	produce_redirect_list(t_parser *state);
t_parser_error	produce_redirect_list_rest(t_parser *state);
t_parser_error	produce_io_redirect(t_parser *state);
t_parser_error	produce_io_file(t_parser *state);
t_parser_error	produce_filename(t_parser *state);
t_parser_error	produce_io_here(t_parser *state);
t_parser_error	produce_here_end(t_parser *state);

#endif // PRODUCTIONS_H
