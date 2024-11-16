#ifndef T_WORD_QUOTES_LIST_H
# define T_WORD_QUOTES_LIST_H

#include "error/t_error.h"
#include "word/t_word_list/t_word_list.h"

#include <stddef.h>

typedef enum e_word_quote_state {
	WQS_SINGLY_QUOTED,
	WQS_DOUBLY_QUOTED,
	WQS_UNQUOTED
} t_word_quote_state;

typedef struct s_word_quotes_list
{
	struct s_word_quotes_list *next;
	enum e_word_quote_state state;
	char *part;
} t_word_quotes_list; // TODO: denest this entire module one level and rename accordingly

t_error wql_parse(const char *compound_word, t_word_quotes_list **out);
t_error wql_new(t_word_quote_state state, char *part, t_word_quotes_list **out);

t_word_quotes_list *wql_last(t_word_quotes_list *wql);

t_error wql_push_back(t_word_quotes_list **wql, t_word_quote_state state, char *part);
void wql_push_back_link(t_word_quotes_list **wql, t_word_quotes_list *link);

void wql_delone(t_word_quotes_list **wql);
void wql_clear(t_word_quotes_list **wql);

t_error wql_make_joined(const t_word_quotes_list *wql, char **out);

t_error make_split_wl_with_ifs(t_word_quotes_list *wql, const char *ifs, t_word_list **out);

#endif // T_WORD_QUOTES_LIST_H