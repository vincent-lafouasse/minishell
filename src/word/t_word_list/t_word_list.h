#pragma once

#include <stddef.h>
#include "error/t_error.h"

typedef void (*t_destructor)(void *);

typedef struct s_word_list
{
	struct s_word_list *next;
	char *contents;
} t_word_list;

t_word_list *wl_new(char *contents);
t_word_list *wl_clone(const t_word_list *words);

size_t wl_len(const t_word_list *words);

t_error wl_push_back(t_word_list **words, char *contents);
void wl_push_back_link(t_word_list **words, t_word_list *link);
t_word_list *wl_pop(t_word_list **words);

void wl_delone(t_word_list **words, t_destructor del);
void wl_clear(t_word_list **words, t_destructor del);

char **wl_into_word_array(t_word_list **words);
