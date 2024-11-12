/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   productions.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:19:01 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 20:05:34 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRODUCTIONS_H
# define PRODUCTIONS_H

# include "../t_parser/t_parser.h"
# include "../t_symbol/t_symbol.h"

t_symbol	produce_redirect_list_rest(t_parser *state);
t_symbol	produce_subshell_precedes(t_parser *state);
t_symbol	produce_io_redirect(t_parser *state);
t_symbol	produce_redirect_list(t_parser *state);
t_symbol	produce_io_file(t_parser *state);
t_symbol	produce_program(t_parser *state);
t_symbol	produce_maybe_cmd_suffix(t_parser *state);
t_symbol	produce_simple_command(t_parser *state);
t_symbol	produce_cmd_prefix_rest(t_parser *state);
t_symbol	produce_cmd_suffix_rest(t_parser *state);
t_symbol	produce_cmd_suffix(t_parser *state);
t_symbol	produce_filename(t_parser *state);
t_symbol	produce_here_end(t_parser *state);
t_symbol	produce_complete_command_rest(t_parser *state);
t_symbol	produce_cmd_prefix(t_parser *state);
t_symbol	produce_cmd_prefix_precedes(t_parser *state);
t_symbol	produce_pipeline(t_parser *state);
t_symbol	produce_pipeline_rest(t_parser *state);
t_symbol	produce_command(t_parser *state);
t_symbol	produce_complete_command(t_parser *state);
t_symbol	produce_io_here(t_parser *state);
t_symbol	produce_subshell(t_parser *state);

#endif
