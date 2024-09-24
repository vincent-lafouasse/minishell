use std::collections::{HashMap, HashSet};

use super::{Grammar, Symbol};

#[derive(Debug)]
pub struct LLProperties {
    first: HashMap<Symbol, HashSet<Symbol>>,
    //follow: HashMap<Symbol, HashSet<Symbol>>,
    //first_augmented: HashMap<(Symbol, Symbol), HashMap<>>
}

impl LLProperties {
    fn compute_first(grammar: &Grammar) -> HashMap<Symbol, HashSet<Symbol>> {
        let mut first = HashMap::new();

        first.insert("$".to_string(), HashSet::from(["$".to_string()]));
        first.insert("".to_string(), HashSet::from(["".to_string()]));
        for terminal in grammar.terminals().clone() {
            first.insert(terminal.clone(), HashSet::from([terminal]));
        }

        for non_terminal in grammar.non_terminals().clone() {
            first.insert(non_terminal, HashSet::new());
        }

        let mut changing = true;
        while changing {
            changing = false;
            for (variable, production) in grammar
                .rules()
                .iter()
                .map(|(symbol, productions)| productions.iter().map(|p| (symbol.clone(), p)))
                .flatten()
            {
                dbg!((&variable, &production));
                // while (FIRST sets are still changing) do;
                //     for each p∈P, where p has the form A→β do;
                //         if β is β1β2...βk, where βi ∈ T ∪NT, then begin;
                //             rhs ← FIRST(β1) − {ε};
                //             i ← 1;
                //             while (ε ∈ FIRST(βi) and i ≤ k-1) do;
                //                 rhs ← rhs ∪ (FIRST(βi+1)−{ε});
                //                 i ← i + 1;
                //             end;
                //         end;
                //         if i = k and ε∈FIRST(βk)
                //             then rhs ← rhs ∪ {ε};
                //         FIRST(A) ← FIRST(A) ∪ rhs;
                //     end;
                // end;
                let mut rhs = HashSet::new();
                //first() -> Option<&String>.as_ref() -> Option<&str>
                let first_or_emptystr = if let Some(f) = production.first() {
                    &f
                } else {
                    ""
                };
                for sym in &first[first_or_emptystr] {
                    rhs.insert(sym.to_owned());
                }
                rhs.remove("");
                for i in 0..production.len().saturating_sub(1) {
                    println!("here");
                    if !first[&production[i]].contains("") {
                        dbg!(&first[&production[i]]);
                        break;
                    }
                    let mut empty_str_excluded = first[&production[i + 1]].clone();
                    dbg!(empty_str_excluded.remove(""));
                    rhs = rhs
                        .union(&empty_str_excluded)
                        .map(String::to_owned)
                        .collect::<HashSet<String>>();
                    if i == production.len().saturating_sub(1) {
                        rhs.insert("".to_owned());
                    }
                    let union = first[&variable]
                        .union(&rhs)
                        .map(String::to_owned)
                        .collect::<HashSet<_>>();
                    if union != first[&variable] {
                        first.insert(variable.clone(), union);
                        changing = true;
                    }
                }
            }
        }

        first
    }

    pub fn compute(grammar: &Grammar) -> Self {
        let first = Self::compute_first(grammar);

        dbg!(LLProperties {
            first,
            //follow: unimplemented!(),
        })
    }
}
