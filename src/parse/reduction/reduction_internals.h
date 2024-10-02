#ifndef REDUCTION_INTERNALS_H
# define REDUCTION_INTERNALS_H

# include "../t_symbol/t_symbol.h"
# include "tokenize/t_token_list/t_token_list.h"

t_token_list *gather_leaves(t_symbol* root);

#endif /* REDUCTION_INTERNALS_H */
