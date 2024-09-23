#![allow(dead_code)]
#![allow(unused_imports)]

mod grammar;

use grammar::{Grammar, Symbol};

fn main() {
	let yacc_grammar = include_str!("../corrected_bash_grammar.y");
	let grammar = Grammar::from_yacc_text(yacc_grammar);
}
