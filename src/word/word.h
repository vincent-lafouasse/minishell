/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:32:49 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:32:54 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_H
# define WORD_H

# include "error/t_error.h"

t_error	word_clone(const char *word, char **out);
void	truncate_to_one_line_if_necessary(char *str);

#endif // WORD_H
