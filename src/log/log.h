/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:11:27 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:11:41 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "execute/t_env/t_env.h"
#include "parse/t_command/t_command.h"
#include "parse/tokenize/t_token_list/t_token_list.h"
#include "parse/t_symbol/t_symbol.h"
#include "word/t_word_list/t_word_list.h"
#include "redirection/t_redir_list/t_redir_list.h"

void log_token(t_token token);
void log_token_list(const t_token_list *tkl);
void parse_tree_to_json(const t_symbol *root);
void syntax_tree_to_json(t_command tree);
void log_word_list(const t_word_list *words);
void log_redir_list(const t_redir_list *rdl);
void log_env(const t_env *env);
