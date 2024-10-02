program               : complete_command
                      ;
complete_command      : pipeline complete_command_rest
                      ;
complete_command_rest : AND_AND pipeline complete_command_rest
                      | OR_OR pipeline complete_command_rest
                      |
                      ;
pipeline              : command pipeline_rest
                      ;
pipeline_rest         : PIPE command pipeline_rest
                      |
                      ;
command               : simple_command
                      | subshell subshell_precedes
                      ;
subshell_precedes     : redirect_list
                      |
                      ;
subshell              : L_PAREN complete_command R_PAREN
                      ;
simple_command        : cmd_prefix cmd_prefix_precedes
                      | WORD maybe_cmd_suffix
                      ;
maybe_cmd_suffix      : cmd_suffix
                      |
                      ;
cmd_prefix_precedes   : WORD maybe_cmd_suffix
                      |
                      ;
cmd_prefix            : io_redirect cmd_prefix_rest
                      ;
cmd_prefix_rest       : io_redirect  cmd_prefix_rest
                      |
                      ;
cmd_suffix            : io_redirect cmd_suffix_rest
                      | WORD cmd_suffix_rest
                      ;
cmd_suffix_rest       : io_redirect cmd_suffix_rest
                      | WORD cmd_suffix_rest
                      |
                      ;
redirect_list         : io_redirect redirect_list_rest
                      ;
redirect_list_rest    : io_redirect redirect_list_rest
                      |
                      ;
io_redirect           : io_file
                      | io_here
                      ;
io_file               : L_ANGLE_BRACKET filename
                      | R_ANGLE_BRACKET filename
                      | DR_ANGLE_BRACKET filename
                      ;
filename              : WORD
                      ;
io_here               : DL_ANGLE_BRACKET here_end
                      ;
here_end              : WORD
                      ;
