#ifndef WORD_H
# define WORD_H

# include "error/t_error.h"

t_error word_clone(const char *word, char **out);
bool word_has_quotes(const char *word);

#endif // WORD_H
