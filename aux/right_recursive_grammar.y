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
complete_command : pipeline
                 | complete_command AND_IF pipeline  /* left recursion */
                 | complete_command OR_IF  pipeline  /* left recursion */
                 ;
pipeline         :              command
                 | pipeline '|' command  /* left recursion */
                 ;
command          : simple_command
                 | subshell
                 | subshell redirect_list
                 ;
subshell         : '(' complete_command ')'
                 ;
wordlist         : WORD rest_of_wordlist
                 ;
rest_of_wordlist : WORD rest_of_wordlist
                 | /* empty */
                 ;
simple_command   : cmd_prefix WORD cmd_suffix
                 | cmd_prefix WORD
                 | cmd_prefix
                 | WORD cmd_suffix
                 | WORD
                 ;
cmd_prefix       :            io_redirect
                 | cmd_prefix io_redirect  /* left recursion */
                 ;
cmd_suffix       :            io_redirect
                 | cmd_suffix io_redirect  /* left recursion */
                 |            WORD
                 | cmd_suffix WORD  /* left recursion */
                 ;
redirect_list    :               io_redirect
                 | redirect_list io_redirect  /* left recursion */
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
