%token NUM

%start expr
%%
expr      : term expr_rest
          ;
expr_rest : + term expr_rest
          | - term expr_rest
          |
          ;
term      : factor term_rest
          ;
term_rest : * factor term_rest
          | / factor term_rest
          |
          ;
factor    : NUM
          | '(' expr ')'
          ;
