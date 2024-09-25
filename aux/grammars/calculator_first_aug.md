## Expr rest

```
expr_rest -> + term expr_rest
+

expr_rest -> - term expr_rest
-

expr_rest -> ε
 $
```

## Factor

```
factor -> '(' expr ')'
'('

factor -> ε
NUM
```

## Term rest

```
term_rest -> * factor term_rest 
*

term_rest -> / factor term_rest 
/

term_rest -> ε
+ $ -
```
