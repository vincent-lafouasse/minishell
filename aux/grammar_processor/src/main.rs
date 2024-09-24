#![allow(dead_code)]
#![allow(unused_imports)]

mod grammar;

use grammar::{Grammar, GrammarRepresentation, LLProperties, Symbol};

fn main() {
    //    let yacc_grammar = "e : t e_prime
    //   ;
    //e_prime : + t e_prime
    //   |
    //   ;
    //t  : f t_prime
    //   ;
    //t_prime : * f t_prime
    //   |
    //   ;
    //f  : ( e )
    //   | ID
    //   ;";
    let yacc_grammar = include_str!("../corrected_bash_grammar.y");
    let grammar = Grammar::from_yacc_text(yacc_grammar);
    //let ll_table = LLTable::new(&grammar).expect("LL table could not be generated from grammar");
    grammar.log_grammar(GrammarRepresentation::Canonical);
    let ll_properties = LLProperties::compute(&grammar);
    //dbg!(grammar);
}
