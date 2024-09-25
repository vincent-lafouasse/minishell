use std::collections::{HashMap, HashSet};
use std::hash::Hash;
use std::rc::Rc;

use super::{Grammar, Symbol};

#[derive(Debug)]
pub struct LLProperties {
    grammar: Rc<Grammar>,
    first: HashMap<Symbol, HashSet<Symbol>>,
    follow: HashMap<Symbol, HashSet<Symbol>>,
    //first_augmented: HashMap<(Symbol, Symbol), HashMap<>>
}

fn ll_first_of_symbol_string(
    string: &[Symbol],
    first: &HashMap<Symbol, HashSet<Symbol>>,
) -> HashSet<Symbol> {
    // HACK: should handle this in parsing
    assert_ne!(string.len(), 0);

    let mut res = first[string.first().unwrap()].clone();
    let mut i = 0;
    while first[&string[i]].contains("") && i < string.len().saturating_sub(1) {
        res = res
            .union(&first[&string[i + 1]].clone())
            .map(String::to_owned)
            .collect::<HashSet<String>>();
        i += 1;
    }
    res
}

fn find_mutually_non_disjoint_sets<T: Eq + Hash>(
    sets: &[HashSet<T>],
) -> Option<(&HashSet<T>, &HashSet<T>)> {
    for (i, a) in sets.iter().enumerate() {
        for (j, b) in sets.iter().enumerate() {
            if i == j {
                continue;
            }

            if a.intersection(b).next().is_some() {
                return Some((a, b));
            }
        }
    }
    None
}

fn owned_cloned_union<T: Clone + Eq + Hash>(lhs: &HashSet<T>, rhs: &HashSet<T>) -> HashSet<T> {
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
                .flat_map(|(symbol, productions)| productions.iter().map(|p| (symbol.clone(), p)))
            {
                let mut rhs = HashSet::new();
                // HACK: should handle this during parsing
                let mut production = production.clone();
                if production.is_empty() {
                    production.push("".to_string());
                }
                // end HACK
                for sym in &first[production.first().unwrap()] {
                    rhs.insert(sym.to_owned());
                }
                rhs.remove("");
                let mut i = 0;
                while first[&production[i]].contains("") && i < production.len().saturating_sub(1) {
                    let mut empty_str_excluded = first[&production[i + 1]].clone();
                    empty_str_excluded.remove("");
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
        follow.insert(
            grammar.start_symbol().clone(),
            HashSet::from(["$".to_owned()]),
        );

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
                .flat_map(|(symbol, productions)| productions.iter().map(|p| (symbol.clone(), p)))
            {
                let mut trailer = follow
                    .get_mut(&variable)
                    .expect("trailer uninitialized")
                    .clone();
                for sym in production.iter().rev() {
                    if grammar.non_terminals().contains(sym) {
                        let entry = follow
                            .get_mut(sym)
                            .unwrap_or_else(|| panic!("entry does not exist for symbol: {sym}"));
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

    pub fn compute(grammar: Rc<Grammar>) -> Self {
        let first = Self::compute_first(&grammar);
        let follow = Self::compute_follow(&grammar, &first);

        LLProperties {
            first,
            follow,
            grammar
        }
    }

    pub fn is_ll_compatible(&self) -> bool {
        for (variable, branches) in self
            .grammar
            .rules()
            .iter()
            .filter(|(_, branches)| branches.len() > 1)
        {
            let mut production_first_augmenteds: Vec<HashSet<Symbol>> = Vec::new();
            for branch in branches {
                let mut branch = branch.clone();
                // HACK: should filter this during parsing
                if branch.is_empty() {
                    branch.push("".to_string())
                }
                // end HACK
                let mut local_first_augmented = ll_first_of_symbol_string(&branch, &self.first);
                if local_first_augmented.contains("") {
                    let variable_follow = self.follow.get(variable).unwrap();
                    local_first_augmented =
                        owned_cloned_union(&local_first_augmented, variable_follow);
                }
                production_first_augmenteds.push(local_first_augmented);
            }

            if let Some((a, b)) = find_mutually_non_disjoint_sets(&production_first_augmenteds) {
                eprintln!("conflict on expansion of {variable} between: ");
                eprintln!("  > a: {a:?}");
                eprintln!("  > b: {b:?}");
                return false;
            };
        }
        true
    }
}
