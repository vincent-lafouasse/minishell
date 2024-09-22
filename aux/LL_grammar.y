%token  WORD


%token  AND_IF    OR_IF
/*      '&&'      '||' */


%token  DLESS  DGREAT  
/*      '<<'   '>>'  */


/* -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- */
%start program
%%
program          : complete_command
                 ;
complete_command : pipeline rest_cmd
                 ;
rest_cmd         : AND_IF rest_cmd
                 | OR_IF  rest_cmd
                 | /* empty */
                 ;
pipeline         : command rest_of_pipeline
                 ;
rest_of_pipeline : '|' rest_of_pipeline
                 | /* empty */
                 ;
command          : simple_command
                 | subshell
                 | subshell redirect_list
                 ;
subshell         : '(' complete_command ')'
                 ;
simple_command   : cmd_prefix WORD cmd_suffix
                 | cmd_prefix WORD
                 | cmd_prefix
                 | WORD cmd_suffix
                 | WORD
                 ;
cmd_prefix       : io_redirect rest_prefix
                 ;
rest_prefix      : io_redirect  rest_prefix
                 | /* empty */
                 ;
cmd_suffix       : io_redirect
                 | WORD
                 | io_redirect rest_of_suffix
                 | WORD rest_of_suffix
                 ;
rest_of_suffix   : io_redirect rest_of_suffix
                 | WORD rest_of_suffix
                 | /* empty */
                 ;
redirect_list    : io_redirect rest_of_redir
                 ;
rest_of_redir    : io_redirect  rest_of_redir
                 | /* empty */
                 ;
io_redirect      :             io_file
                 |             io_here
                 ;
io_file          : '<'       filename
                 | '>'       filename
                 | DGREAT    filename
                 ;
filename         : WORD
                 ;
io_here          : DLESS     here_end
                 ;
here_end         : WORD
                 ;
