%token  WORD
%token  ASSIGNMENT_WORD
%token  NAME
%token  NEWLINE


%token  AND_IF    OR_IF
/*      '&&'      '||'  */


%token  DLESS  DGREAT
/*      '<<'   '>>' */


/* -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- */
%start program
%%
program          : complete_command
                 | /* empty */
                 ;
complete_command : list separator_op
                 | list
                 ;
list             : list separator_op and_or
                 |                   and_or
                 ;
and_or           :               pipeline
                 | and_or AND_IF pipeline
                 | and_or OR_IF  pipeline
                 ;
pipeline         :              command
                 | pipeline '|' command
                 ;
command          : simple_command
                 | subshell
                 | subshell redirect_list
                 ;
subshell         : '(' compound_list ')'
                 ;
compound_list    : term
                 | term separator
                 ;
term             : term separator and_or
                 |                and_or
                 ;
name             : NAME                     /* Apply rule 5 */
                 ;
wordlist         : wordlist WORD
                 |          WORD
                 ;
pattern_list     :                  WORD    /* Apply rule 4 */
                 |              '(' WORD    /* Do not apply rule 4 */
                 | pattern_list '|' WORD    /* Do not apply rule 4 */
                 ;
simple_command   : cmd_prefix cmd_word cmd_suffix
                 | cmd_prefix cmd_word
                 | cmd_prefix
                 | cmd_name cmd_suffix
                 | cmd_name
                 ;
cmd_name         : WORD                   /* Apply rule 7a */
                 ;
cmd_word         : WORD                   /* Apply rule 7b */
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
separator_op     : '&'
                 | ';'
                 ;
separator        : separator_op
                 ;
sequential_sep   : ';'
                 ;
