use std::collections::{HashMap, HashSet};

pub type Symbol = String;

#[derive(Debug)]
pub struct Grammar {
    non_terminals: HashSet<Symbol>,
    terminals: HashSet<Symbol>,
    /// key is always NT in CFG
    rules: HashMap<Symbol, HashSet<Vec<Symbol>>>,
    start_symbol: Symbol,
}

#[derive(Clone, Copy)]
pub enum GrammarRepresentation {
    Yacc,
}

impl Grammar {
    pub fn non_terminals(&self) -> &HashSet<Symbol> {
        &self.non_terminals
    }

    pub fn terminals(&self) -> &HashSet<Symbol> {
        &self.terminals
    }

    pub fn rules(&self) -> &HashMap<Symbol, HashSet<Vec<Symbol>>> {
        &self.rules
    }

    pub fn start_symbol(&self) -> &Symbol {
        &self.start_symbol
    }

    pub fn from_yacc_file<R: std::io::Read>(mut reader: R) -> Grammar {
        let contents_owned = {
            let mut buf = String::new();
            reader
                .read_to_string(&mut buf)
                .expect("can no read to string");
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
        for rule in &rules {
            match &rule[..] {
                &[variable, productions] => {
                    let branch_set: &mut HashSet<Vec<Symbol>> =
                        rules_map.entry(variable.trim().to_owned()).or_default();
                    for branch in productions.split("|").map(|r| r.trim()) {
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

        let all_symbols: HashSet<Symbol> =
            rules_map
                .clone()
                .into_iter()
                .fold(HashSet::new(), |mut set, (production, rules)| {
                    set.insert(production);
                    set.extend(rules.into_iter().flatten());
                    set
                });

        let (non_terminals, terminals): (HashSet<Symbol>, HashSet<Symbol>) = all_symbols
            .into_iter()
            .partition(|sym| rules_map.contains_key(sym));

        Grammar {
            terminals,
            non_terminals,
            start_symbol: start_symbol.to_string(),
            rules: rules_map,
        }
    }

    pub fn log_grammar(&self, repr: GrammarRepresentation) {
        let repr = GrammarRepresentation::Yacc else {
            unreachable!();
        };
        let start_symbol = &self.start_symbol;

        let alignment = self
            .non_terminals
            .iter()
            .map(String::len)
            .max()
            .expect("grammar is empty");

        fn log_rule(
            name: &str,
            branches: &HashSet<Vec<Symbol>>,
            repr: GrammarRepresentation,
            align: usize,
        ) {
            let empty_padding = " ".repeat(align + 1);
            let padded_name = name.to_string() + &" ".repeat(align - name.len() + 1);
            let branch_separator = "\n".to_string() + &empty_padding + "| ";
            let formatted_branches = branches
                .iter()
                .map(|symbols| symbols.join(" "))
                .collect::<Vec<String>>()
                .join(&branch_separator);
            let end_of_rules = empty_padding + "; ";
            print!("{padded_name}: {formatted_branches} \n{end_of_rules} \n");
        }

        log_rule(start_symbol, &self.rules[start_symbol], repr, alignment);

        for (variable, productions) in &self.rules {
            if variable == start_symbol {
                continue;
            }
            log_rule(variable, productions, repr, alignment);
        }
    }
}
