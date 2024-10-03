#ifndef REDUCTION_INTERNALS_H
# define REDUCTION_INTERNALS_H

# include "../t_command/t_command.h"
# include "../t_symbol/t_symbol.h"
# include "tokenize/t_token_list/t_token_list.h"
# include "redirection/t_redir_list/t_redir_list.h"
# include "word/t_word_list/t_word_list.h"

t_token_list *gather_leaves(t_symbol *root);

t_command	reduce_complete_command(t_symbol *root);
t_command	reduce_pipeline(t_symbol *pipeline);
t_command	reduce_command(t_symbol *command);
t_command	reduce_subshell(t_symbol *subshell, t_symbol *trailing_redirs);
t_command	reduce_simple_command(t_symbol *simple_cmd);
void		reduce_simple_command_like(t_symbol *symbol, t_word_list **words, \
							t_redir_list **redirections);

#endif /* REDUCTION_INTERNALS_H */