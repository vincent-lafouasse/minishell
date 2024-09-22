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
complete_command : pipeline r_complete_cmd
                 ;
r_complete_cmd   : AND_IF pipeline r_complete_cmd
                 | OR_IF  pipeline r_complete_cmd
                 | /* empty */
                 ;
pipeline         : command r_pipeline
                 ;
r_pipeline       : '|' command r_pipeline
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
cmd_prefix       :            io_redirect
                 | cmd_prefix io_redirect
                 ;
cmd_suffix       :            io_redirect
                 | cmd_suffix io_redirect
                 |            WORD
                 | cmd_suffix WORD
                 ;
redirect_list    :               io_redirect
                 | redirect_list io_redirect
                 ;
io_redirect      :             io_file
                 |             io_here
                 ;
io_file          : '<'       filename
                 | '>'       filename
                 | DGREAT    filename
                 ;
filename         : WORD                      /* Apply rule 2 */
                 ;
io_here          : DLESS     here_end
                 ;
here_end         : WORD                      /* Apply rule 3 */
                 ;
