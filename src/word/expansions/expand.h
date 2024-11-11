#ifndef EXPAND_H
# define EXPAND_H

# include "../t_word_list/t_word_list.h"
# include "execute/t_env/t_env.h"
# include "execute/execute.h"

t_error perform_all_expansions_on_words(t_word_list *word);

t_error expand_env_variables(t_word_list* words, t_state* state);
t_error expand_single_word(char* word, t_state* state);

t_error path_expanded_word(const t_env *env, const char *word, char **out);
char *quote_removed_word(const char *word);

#endif // EXPAND_H
