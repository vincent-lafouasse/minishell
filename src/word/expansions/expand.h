#ifndef EXPAND_H
# define EXPAND_H

# include "../t_word_list/t_word_list.h"
# include "execute/t_env/t_env.h"
# include "execute/execute.h"

typedef struct s_expansion_variables {
	const t_env *env;
	int last_status;
} t_expansion_variables;

t_error perform_all_expansions_on_words(t_word_list *word);

t_error expand_env_variables(t_expansion_variables vars, t_word_list* words);
t_error variable_expand_word(t_expansion_variables vars, const char *word, char **out);

t_error path_expanded_word(const t_env *env, const char *word, char **out);
char *quote_removed_word(const char *word);

#endif // EXPAND_H
