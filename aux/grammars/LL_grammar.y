%token  WORD

/* -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- */
%start program
%%
program               : complete_command
                      ;
complete_command      : pipeline complete_command_rest
                      ;
complete_command_rest : '&&' pipeline complete_command_rest
                      | '||' pipeline complete_command_rest
                      | /* empty */
                      ;
pipeline              : command pipeline_rest
                      ;
pipeline_rest         : '|' command pipeline_rest
                      | /* empty */
                      ;
command               : simple_command
                      | subshell subshell_precedes
                      ;
subshell_precedes     : redirect_list
                      | /* empty */
                      ;
subshell              : '(' complete_command ')'
                      ;
simple_command        : cmd_prefix cmd_prefix_precedes
                      | WORD maybe_cmd_suffix
                      ;
maybe_cmd_suffix      : cmd_suffix
                      | /* empty */
                      ;
cmd_prefix_precedes   : WORD maybe_cmd_suffix
                      | /* empty */
                      ;
cmd_prefix            : io_redirect cmd_prefix_rest
                      ;
cmd_prefix_rest       : io_redirect  cmd_prefix_rest
                      | /* empty */
                      ;
cmd_suffix            : io_redirect cmd_suffix_rest
                      | WORD cmd_suffix_rest
                      ;
cmd_suffix_rest       : io_redirect cmd_suffix_rest
                      | WORD cmd_suffix_rest
                      | /* empty */
                      ;
redirect_list         : io_redirect redirect_list_rest
                      ;
redirect_list_rest    : io_redirect redirect_list_rest
                      | /* empty */
                      ;
io_redirect           : io_file
                      | io_here
                      ;
io_file               : '<' filename
                      | '>' filename
                      | '>>' filename
                      ;
filename              : WORD                             /* Apply rule 2 */
                      ;
io_here               : '<<' here_end
                      ;
here_end              : WORD                             /* Apply rule 3 */
                      ;
