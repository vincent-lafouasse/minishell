use std::rc::Rc;

mod grammar;

use grammar::{Grammar, GrammarRepresentation, LLProperties};

const _JS_MACHINES_GRAMMAR: &str = "e : t e_prime
    ;
e_prime : + t e_prime
    |
    ;
t  : f t_prime
    ;
t_prime : * f t_prime
    |
    ;
f  : ( e )
    | ID
;";

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

fn main() {
    let yacc_grammar = include_str!("../corrected_bash_grammar.y");
    let grammar = Rc::new(Grammar::from_yacc_text(yacc_grammar));
    //let ll_table = LLTable::new(&grammar).expect("LL table could not be generated from grammar");
    grammar.log_grammar(GrammarRepresentation::Canonical);
    let ll_properties = LLProperties::compute(grammar.clone());
    assert!(ll_properties.is_ll_compatible());
    //dbg!(grammar);
}
