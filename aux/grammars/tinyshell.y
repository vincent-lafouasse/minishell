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
complete_command_rest : AND pipeline complete_command_rest
                      | OR pipeline complete_command_rest
                      |
                      ;
pipeline              : command pipeline_rest
                      ;
pipeline_rest         : PIPE command pipeline_rest
                      |
                      ;
command               : word_list
                      ;
word_list             : WORD word_list_rest
                      ;
word_list_rest        : WORD word_list_rest
                      |
                      ;
