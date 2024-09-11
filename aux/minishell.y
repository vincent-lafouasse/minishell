%token WORD /* terminal symbols */
%token PIPE
%token OR
%token AND

/* terminal symbols */
%start command
%%
command         : compound_cmd
                ;


compound_cmd    : pipe_cmd
                | logical_cmd
                | simple_cmd
                ;

pipe_cmd        : compound_cmd PIPE compound_cmd

logical_cmd     : compound_cmd AND compound_cmd
                | compound_cmd OR compound_cmd
                ;

word_list       : word_list WORD
                | WORD
                ;
