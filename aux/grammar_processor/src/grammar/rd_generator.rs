use std::collections::HashSet;
use std::fs::File;
use std::io::{self, Write};
use std::path::Path;

use super::Symbol;

use super::LLProperties;

#[derive(Debug)]
pub struct RdGenerator {
    ll_properties: LLProperties,
}

const EOF_TOKEN_NAME: &str = "EOF_TOKEN";

impl RdGenerator {
	/// `ll_properties` must be generated from a backtrack-free LL(1) compatible
	/// grammar
    pub fn new(ll_properties: LLProperties) -> Option<Self> {
        if ll_properties.is_ll_compatible() {
            Some(Self { ll_properties })
        } else {
            None
        }
    }

    fn write_header_files(&self, output_path: &Path) -> io::Result<()> {
        let mut file = File::create(output_path.join("productions.h"))?;

        writeln!(file, "#pragma once")?;
        writeln!(file)?;
        for variable in self.ll_properties.underlying_grammar().rules().keys() {
            writeln!(file, "t_symbol	produce_{variable}(t_parser *state);")?;
        }

        Ok(())
    }

    fn write_symbol_allocation_and_error_handling(
        &self,
        out: &mut dyn Write,
        symbol_kind: &str,
        production_max_size: usize,
    ) -> io::Result<()> {
        writeln!(
            out,
            "	symbol = symbol_new_non_terminal({symbol_kind}, {production_max_size});"
        )?;
        writeln!(out, "	if (symbol.production == NULL)")?;
        writeln!(out, "	{{")?;
        writeln!(out, "		state->err = E_OOM;")?;
        writeln!(out, "		return symbol;")?;
        writeln!(out, "	}}")?;

        Ok(())
    }

    fn write_rule_match_condition(
        &self,
        out: &mut dyn Write,
        variable: &Symbol,
        rule: &[Symbol],
        is_first_production: bool,
    ) -> io::Result<()> {
        let if_or_else_if = if is_first_production { "if" } else { "else if" };

        let mut first_aug = self.ll_properties.get_first_augmented(variable, rule);
		// the empty string does not denominate a token; simply ensure the next
		// token in the stream is indeed the one we expect it to be (i.e. if it
		// exists in the FOLLOW set of `variable`)
		first_aug.remove("");
		// HACK
		if first_aug.contains("$") {
			first_aug.remove("$");
			first_aug.insert(EOF_TOKEN_NAME.to_string());
		}
		// end HACK
        let matching_token_count = first_aug.len();
        let matching_token_string = first_aug.into_iter().collect::<Vec<String>>().join(", ");
        let condition = format!(
            "parser_matches_one_of(state, (t_token_type[]){{{}}}), {})",
            matching_token_string, matching_token_count
        );

        writeln!(out, "	{if_or_else_if} ({condition})")
    }

    fn write_rule_body(
        &self,
        out: &mut dyn Write,
        symbol: &Symbol,
        indent_count: usize,
        is_final: bool,
    ) -> io::Result<()> {
        let is_terminal = self
            .ll_properties
            .underlying_grammar()
            .terminals()
            .contains(symbol);
        let accept_or_produce = if is_terminal { "accept" } else { "produce" };
        let type_or_production = if is_terminal {
            symbol.to_uppercase()
        } else {
            format!("produce_{symbol}")
        };
        let call = format!(
            "parser_{accept_or_produce}_push(state, {type_or_production}, symbol.production)"
        );
        let indents = "\t".repeat(indent_count);

        if is_final {
            writeln!(out, "{indents}{call};")
        } else {
            writeln!(out, "{indents}if ({call})")
        }
    }

    fn write_production(
        &self,
        out: &mut dyn Write,
        (variable, production_rules): (&Symbol, &HashSet<Vec<Symbol>>),
    ) -> io::Result<()> {
        writeln!(out, "#include \"production_rules.h\"")?;
        writeln!(out, "#include \"t_symbol.h\"")?;
        writeln!(out, "#include \"t_parser.h\"")?;
        writeln!(out)?;
        writeln!(out, "t_symbol	produce_{variable}(t_parser *state)")?;
        writeln!(out, "{{")?;
        writeln!(out, "	t_symbol	symbol;")?;
        writeln!(out)?;
        let symbol_kind = variable.to_uppercase();
        let production_max_size = production_rules
            .iter()
            .map(Vec::len)
            .max()
            .expect("no production_rules");
        self.write_symbol_allocation_and_error_handling(
            out,
            symbol_kind.as_str(),
            production_max_size,
        )?;
        let is_single_rule_production = production_rules.len() == 1;
        for (i, rule) in production_rules.iter().enumerate() {
            if !is_single_rule_production {
                let is_first_production = i == 0;
                self.write_rule_match_condition(out, variable, rule, is_first_production)?;
                writeln!(out, "	{{")?;
            }
            let rule_symbol_count = rule.len();
            for (j, symbol) in rule.iter().enumerate() {
                let is_final = j == rule_symbol_count - 1;
                let indent_count = j + if is_single_rule_production { 1 } else { 2 };
                self.write_rule_body(out, symbol, indent_count, is_final)?;
            }
            if !is_single_rule_production {
                writeln!(out, "	}}")?;
            }
        }
        writeln!(out, "	return (symbol);")?;
        writeln!(out, "}}")?;

        Ok(())
    }

    pub fn generate(&self, output_path: &Path) -> io::Result<()> {
        std::fs::create_dir(output_path).ok();
        self.write_header_files(output_path)?;
        for (variable, rule) in self.ll_properties.underlying_grammar().rules() {
            let mut file = File::create(output_path.join(format!("produce_{variable}.c")))?;
            self.write_production(&mut file, (variable, rule))?;
        }

        Ok(())
    }
}
