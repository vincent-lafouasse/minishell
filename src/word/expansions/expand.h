/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:43:58 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 19:44:31 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../t_word_list/t_word_list.h"
# include "execute/t_env/t_env.h"

typedef struct s_expansion_variables
{
	const t_env	*env;
	int			last_status;
}				t_expansion_variables;

t_error	variable_expand_words(t_expansion_variables vars,
			t_word_list **words);
t_error	variable_expand_word(t_expansion_variables vars,
			const char *word, char **out);

t_error	quote_removed_word(const char *word, char **out);
t_error	path_expanded_word(const t_env *env, const char *word,
			char **out);

#endif // EXPAND_H
