%token  WORD


%token  AND_IF    OR_IF
/*      '&&'      '||' */


%token  DLESS  DGREAT  
/*      '<<'   '>>'  */

%token PIPE
%token GREAT
%token LESS
%token LPAREN
%token RPAREN


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
r_pipeline       : PIPE command r_pipeline
                 | /* empty */
                 ;
command          : simple_command
                 | subshell
                 | subshell redirect_list
                 ;
subshell         : LPAREN complete_command RPAREN
                 ;
simple_command   : cmd_prefix WORD cmd_suffix
                 | cmd_prefix WORD
                 | cmd_prefix
                 | WORD cmd_suffix
                 | WORD
                 ;
cmd_prefix       : io_redirect r_cmd_prefix
                 ;
r_cmd_prefix     : io_redirect r_cmd_prefix
                 | /* empty */
                 ;
cmd_suffix       : io_redirect r_cmd_suffix
                 | WORD r_cmd_suffix
                 ;
r_cmd_suffix     : io_redirect r_cmd_suffix 
                 | WORD r_cmd_suffix 
                 | /* empty */
                 ;
redirect_list    : io_redirect r_redir_list
                 ;
r_redir_list     : io_redirect r_redir_list
                 | /* empty */
                 ;
io_redirect      : io_file
                 | io_here
                 ;
io_file          : GREAT     filename
                 | LESS      filename
                 | DGREAT    filename
                 ;
filename         : WORD                      /* Apply rule 2 */
                 ;
io_here          : DLESS     here_end
                 ;
here_end         : WORD                      /* Apply rule 3 */
                 ;
