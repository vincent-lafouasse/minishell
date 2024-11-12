/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:53:12 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 13:53:13 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "error/t_error.h"
#include "t_token_list/t_token_list.h"

//!  Split a string into tokens, mostly words, strings and operators
/*!
  word and string tokens own a copy of their lexeme
*/
t_error	tokenize(const char *input, t_token_list **out);
