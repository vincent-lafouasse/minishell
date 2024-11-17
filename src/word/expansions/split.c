#include "expand_internals.h"

#include "word/t_string/t_string.h"

#include "libft/string.h"
#include "libft/ft_string.h"

#include <stdlib.h>

bool quote_list_expands_to_no_words(const t_word_quotes_list *wql)
{
	const t_word_quotes_list *curr;
	size_t current_part_len;

	curr = wql;
	while (curr)
	{
		if (curr->state != WQS_UNQUOTED)
			return (false);
		current_part_len = ft_strlen(curr->part);
		if (current_part_len > 0)
			return false;
		curr = curr->next;
	}
	return (true);
}

static t_error wl_push_back_empty_string(t_word_list **wl)
{
	char *empty;

	empty = ft_strdup("");
	if (!empty)
		return (E_OOM);
	return (wl_push_back(wl, empty));
}

static t_error append_string_to_end_of_wl(t_word_list **wl, const char *str)
{
	t_string *string;
	char *c_str;
	t_word_list *last;
	t_error err;

	last = *wl;
	while (last && last->next)
		last = last->next;
	string = string_new();
	if (!string)
		return (E_OOM);
	if (last->contents != NULL)
	{
		if (string_extend(&string, last->contents) == true)
			return (E_OOM);
	}
	if (string_extend(&string, str) == true)
		return (E_OOM);
	err = string_make_c_string(string, &c_str);
	if (err != NO_ERROR)
		return (err);
	string_destroy(string);
	free(last->contents);
	last->contents = c_str;
	return (NO_ERROR);
}

static t_error append_unquoted_word_parts(t_word_list **wl, const char *part, \
										  const char *pattern, bool *should_make_new_word)
{
	size_t i;
	bool should_make_new_word;
	char *substring;
	t_error err;

	i = 0;
	while (part[i])
	{
		should_make_new_word = ft_strchr(pattern, part[i]) != NULL;
		while (part[i] != '\0' && ft_strchr(pattern, part[i]) != NULL)
			i++;
		if (should_make_new_word && part[i] == '\0') // bad
			return (wl_push_back_empty_string(wl));
		size_t old_index = i;
		while (part[i] != '\0' && ft_strchr(pattern, part[i]) == NULL)
			i++;
		substring = ft_substr(part, old_index, i - old_index);
		if (!substring)
			return (E_OOM);
		if (should_make_new_word)
			err = wl_push_back(wl, substring);
		else
			err = append_string_to_end_of_wl(wl, substring);
		if (err != NO_ERROR)
			return (err);
	}
	return (NO_ERROR);
}

t_error wql_make_split_wl(const t_word_quotes_list *wql, const char *pattern, t_word_list **out)
{
	t_word_list *head;
	t_error err;

	head = NULL;
	while (wql)
	{
		if (wql->quotes_removed)
			err = append_string_to_end_of_wl(&head, wql->part);
		else
			err = append_unquoted_word_parts(&head, wql->part, pattern);
		if (err != NO_ERROR)
		{
			wl_clear(&head, free);
			return (E_OOM);
		}
		wql = wql->next;
	}
	*out = head;
	return (NO_ERROR);
}
