use std::rc::Rc;

mod grammar;

use grammar::{Grammar, GrammarRepresentation, LLProperties};

const _JS_MACHINES_GRAMMAR: &str = "
e       : t e_prime
        ;
e_prime : + t e_prime
        | - t e_prime
        |
        ;
t       : f t_prime
        ;
t_prime : * f t_prime
        | / f t_prime
        |
        ;
f       : ( e )
        | NUM
        ;
";

const _GEEKS_FOR_GEEKS_GRAMMAR: &str = "E : TE' ;
E' : + TE' | ;
T : FT' ;
T' : * FT' | ;
F : id | (E) ;";

const _SLIDE_ARITHMETIC_GRAMMAR: &str = "Exp : Exp2 Exp' ;
Exp'  : + Exp2 Exp'
      | Exp2 Exp'
      |
      ;
Exp2  : Exp3 Exp2' ;
Exp2' : * Exp3 Exp2'
      | / Exp3 Exp2'
      |
      ;
Exp3  : num
      | ( Exp )
      ;";

const _RIGHT_RECURSIVE_NOT_LL: &str = "
goal : expr ;

expr : term + expr | term - expr | term ;

term : factor * term | factor / term | factor ;

factor : NUM | ID ;
";

fn main() {
    let yacc_grammar = include_str!("../../grammars/tinyshell.y");
    let grammar = Rc::new(dbg!(Grammar::from_yacc_text(yacc_grammar)));
    grammar.log_grammar(GrammarRepresentation::Canonical);

    let ll_properties = LLProperties::compute(grammar.clone());
    if ll_properties.is_ll_compatible() {
        println!("LL(1) compatible");
    } else {
        println!("rip bozo");
    }
}
