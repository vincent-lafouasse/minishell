/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduction_internals.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:25:56 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 14:25:57 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDUCTION_INTERNALS_H
# define REDUCTION_INTERNALS_H

# include "../t_command/t_command.h"
# include "../t_symbol/t_symbol.h"
# include "../tokenize/t_token_list/t_token_list.h"
# include "io/t_redir_list/t_redir_list.h"
# include "word/t_word_list/t_word_list.h"

t_error	gather_leaves(t_symbol *root, t_token_list **out);

t_error	reduce_complete_command(t_symbol *root, t_command *out);
t_error	reduce_pipeline(t_symbol *pipeline, t_command *out);
t_error	reduce_command(t_symbol *command, t_command *out);
t_error	reduce_subshell(t_symbol *subshell, t_symbol *trailing_redirs,
			t_command *out);
t_error	reduce_simple_command(t_symbol *simple_cmd, t_command *out);
t_error	reduce_simple_command_like(t_symbol *symbol, t_word_list **words,
			t_redir_list **redirections);

t_error	invert_conditional_associativity(t_conditional **out);

#endif /* REDUCTION_INTERNALS_H */
