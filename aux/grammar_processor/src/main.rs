#![allow(dead_code)]
#![allow(unused_imports)]
use std::collections::{HashMap, HashSet};

type Symbol = /* Either<NonTerminal, Terminal> */ String;

#[derive(Debug)]
struct Grammar {
    /// key is always NT in CFG
    rules: HashMap<Symbol, HashSet<Vec<Symbol>>>,
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
        let mut this = Self {
            rules: HashMap::new(),
        };

        let rules: Vec<&str> = contents.split(";").collect();
        for rule in rules {
            match &rule.split(":").collect::<Vec<_>>()[..] {
                &[variable, productions] => {
                    let handle = this.rules.entry(variable.trim().to_owned()).or_default();
                    for branch in productions.split("|").map(|r| r.trim()) {
                        let did_not_exist = handle.insert(
                            branch.split_whitespace().map(|s| s.to_owned()).collect::<Vec<String>>()
                        );
                        if !did_not_exist {
                            eprintln!("warning: encountered duplicate branch");
                        }
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

        this
    }
}

fn main() {
    let yacc_grammar = include_str!("../corrected_bash_grammar.y");
    let grammar = Grammar::from_yacc_text(yacc_grammar);
}
