use std::collections::{HashMap, HashSet};

use super::{Grammar, Symbol};

#[derive(Debug)]
pub struct LLProperties {
    first: HashMap<Symbol, HashSet<Symbol>>,
    follow: HashMap<Symbol, HashSet<Symbol>>,
    //first_augmented: HashMap<(Symbol, Symbol), HashMap<>>
}

fn owned_cloned_union<T: Clone +Eq + std::hash::Hash>(lhs: &HashSet<T>, rhs: &HashSet<T>) -> HashSet<T> {
    lhs.union(rhs).map(Clone::clone).collect()
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
        let mut changing = true;
        while changing {
            changing = false;
            for (variable, production) in grammar
                .rules()
                .iter()
                .map(|(symbol, productions)| productions.iter().map(|p| (symbol.clone(), p)))
                .flatten()
            {
                let mut rhs = HashSet::new();
                let mut production = production.clone();
                if production.is_empty() {
                    production.push("".to_string());
                }
                for sym in &first[production.first().unwrap()] {
                    rhs.insert(sym.to_owned());
                }
                rhs.remove("");
                let mut i = 0;
                while first[&production[i]].contains("") && i < production.len().saturating_sub(1) {
                    let mut empty_str_excluded = first[&production[i + 1]].clone();
                    rhs = rhs
                        .union(&empty_str_excluded)
                        .map(String::to_owned)
                        .collect::<HashSet<String>>();
                    i += 1;
                }
                if first[&production[i]].contains("") && i == production.len().saturating_sub(1) {
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

        first
    }

    fn compute_follow(
        grammar: &Grammar,
        first: &HashMap<Symbol, HashSet<Symbol>>,
    ) -> HashMap<Symbol, HashSet<Symbol>> {
        let mut follow = HashMap::new();

        for non_terminal in grammar.non_terminals().clone() {
            follow.insert(non_terminal, HashSet::new());
        }
        follow.insert(grammar.start_symbol().clone(), HashSet::from(["$".to_owned()]));

        // for each A∈NT do;
        //     FOLLOW(A) ← ∅;
        // end;
        //
        // FOLLOW(S) ← {eof};
        //
        // while (FOLLOW sets are still changing) do;
        //     for each p∈P of the form A→β1β2···βk do;
        //         TRAILER ← FOLLOW(A);
        //         for i ← k down to 1 do;
        //             if βi ∈ NT then begin;
        //                 FOLLOW(βi) ← FOLLOW(βi) ∪ TRAILER;
        //                 if ε ∈ FIRST(βi)
        //                     then TRAILER ← TRAILER ∪ (FIRST(βi) − ε);
        //                     else TRAILER ← FIRST(βi);
        //             end;
        //             else TRAILER ← FIRST(βi); // is {βi}
        //         end;
        //     end;
        // end;
        let mut changing = true;
        while changing {
            changing = false;
            for (variable, production) in grammar
                .rules()
                .iter()
                .map(|(symbol, productions)| productions.iter().map(|p| (symbol.clone(), p)))
                .flatten()
            {
                let mut trailer = follow.get_mut(&variable).expect("trailer uninitialized").clone();
                for sym in production.into_iter().rev() {
                    if grammar.non_terminals().contains(sym) {
                        let entry = follow.get_mut(sym).expect(&format!("entry does not exist for symbol: {sym}"));
                        let union = owned_cloned_union(entry, &trailer);
                        if entry != &union {
                            changing = true;
                        }
                        *entry = union;
                        let mut first_of_sym = first.get(sym).unwrap().clone();
                        if first_of_sym.contains("") {
                            first_of_sym.remove("");
                            trailer = owned_cloned_union(&trailer, &first_of_sym);
                        } else {
                            trailer = first_of_sym;
                        }
                    } else {
                        trailer = HashSet::from([sym.clone()]);
                    }
                }
            }
        }

        follow
    }

    pub fn compute(grammar: &Grammar) -> Self {
        let first = Self::compute_first(grammar);
        let follow = Self::compute_follow(grammar, &first);

        dbg!(LLProperties {
            first,
            follow,
        })
    }
}
