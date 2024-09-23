#![allow(dead_code)]
#![allow(unused_imports)]
use std::collections::{HashMap, HashSet};

type Symbol = /* Either<NonTerminal, Terminal> */ String;

#[derive(Debug)]
struct Grammar {
    /// key is always NT in CFG
    rules: HashMap<Symbol, HashSet<Vec<Symbol>>>,
    start_symbol: Symbol,
}

impl Grammar {
    pub fn from_yacc_file<R: std::io::Read>(mut reader: R) -> Grammar {
        let contents_owned = {
            let mut buf = String::new();
            reader.read_to_string(&mut buf).expect("can no read to string");
            buf
        };

        Self::from_yacc_text(&contents_owned)
    }

    pub fn from_yacc_text(contents: &str) -> Grammar {
        let mut rules_map = HashMap::new();

        let rules: Vec<Vec<&str>> = contents
            .split(';')
            .map(|rule| rule.split(':').collect())
            .collect();
        let start_symbol: &str = rules
            .first()
            .and_then(|v| v.first())
            .expect("no rules")
            .trim();
        for rule in rules {
            match &rule[..] {
                &[variable, productions] => {
                    let branch_set: &mut HashSet<Vec<Symbol>> = rules_map.entry(variable.trim().to_owned()).or_default();
                    for branch in productions.split('|').map(|r| r.trim()) {
                        branch_set.insert(
                            branch
                                .split_whitespace()
                                .map(|s| s.to_owned())
                                .collect::<Vec<String>>(),
                        );
                    }
                }
                unknown => {
                    if unknown == ["\n"] {
                        continue;
                    }
                    eprintln!("warning: invalid production");
                }
            };
        }

        Grammar {
            start_symbol: start_symbol.to_string(),
            rules: rules_map,
        }
    }
}

fn main() {
    let yacc_grammar = include_str!("../corrected_bash_grammar.y");
    let grammar = Grammar::from_yacc_text(yacc_grammar);
}
