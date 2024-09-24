#![allow(dead_code)]
#![allow(unused_imports)]

mod grammar;

use grammar::{Grammar, GrammarRepresentation, LLProperties, Symbol};

fn main() {
    //let yacc_grammar = include_str!("../corrected_bash_grammar.y");
    let yacc_grammar = "e : t e_prime
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

    let grammar = Grammar::from_yacc_text(yacc_grammar);
    grammar.log_grammar(GrammarRepresentation::Yacc);
    let ll_properties = LLProperties::compute(&grammar);
}
