#pragma once

#include <stddef.h>
#include "error/t_error.h"

typedef void (*t_destructor)(char *);

typedef enum e_redir_kind {
	FROM_FILE,
	INTO_FILE,
	APPEND_INTO_FILE,
	HERE_DOCUMENT,
} t_redir_kind;

typedef struct s_here_doc {
	char *contents; // null right after parsing
	char *here_doc_eof; // pre-expansion
} t_here_doc;

typedef struct s_redirect {
	t_redir_kind kind;
	union {
		char *filename;
		t_here_doc doc;
	};
} t_redirect;

typedef struct s_redir_list
{
	struct s_redir_list *next;
	t_redirect redirect;
} t_redir_list;

t_redir_list *rdl_new(char *contents);
t_redir_list *rdl_clone(const t_redir_list *words);

size_t rdl_len(const t_redir_list *words);

// used in reduction.c
t_error rdl_push_back(t_redir_list **words, char *contents);
void rdl_push_back_link(t_redir_list **words, t_redir_list *link);
t_redir_list *rdl_pop(t_redir_list **words);

void rdl_delone(t_redir_list **words, t_destructor del);
void rdl_clear(t_redir_list **words, t_destructor del);

char **rdl_into_word_array(t_redir_list **words);
