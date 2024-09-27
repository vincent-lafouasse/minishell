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
command               : word_list
                      ;
word_list             : WORD word_list_rest
                      ;
word_list_rest        : WORD word_list_rest
                      |
                      ;
