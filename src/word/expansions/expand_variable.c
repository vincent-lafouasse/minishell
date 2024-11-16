#include "./expand.h"
#include "./expand_internals.h"

#include "word/t_string/t_string.h"
#include "execute/t_env/t_env.h"
#include "error/t_error.h"

#include "libft/string.h"
#include "libft/ctype.h"
#include "libft/ft_string.h"

#include <stdlib.h>

static size_t identifier_len(const char *str) {
	size_t len = 0;

	if (ft_isdigit(*str))
		return 0;
	while (ft_isalnum(str[len]) || str[len] == '_') {
		len++;
	}
	return (len);
}

static bool is_valid_dollar_variable(const char *substring)
{
	return (*substring == '$' && identifier_len(substring + 1) > 0);
}


static t_error expand_dollar_variable(t_expansion_variables vars, \
									  const char *start, t_string **out)
{
	char *variable_name;
	const char *after_dollar;
	const t_env_entry *variable;
	char *replace_with;
	t_error err;

	after_dollar = start + 1;
	variable_name = ft_substr(after_dollar, 0, identifier_len(after_dollar));
	if (!variable_name)
		return (E_OOM);
	if (*after_dollar == '?')
		replace_with = ft_itoa(vars.last_status);
	else
	{
		variable = env_get(vars.env, variable_name);
		if (variable)
			replace_with = ft_strdup(variable->value);
		else
			replace_with = ft_strdup("");
	}
	if (!replace_with)
	{
		free(variable_name);
		return (E_OOM);
	}
	err = string_extend(out, replace_with);
	free(variable_name);
	free(replace_with);
	return (err);
}

static t_error variable_expand_word_quote(t_expansion_variables vars, \
										  t_word_quotes_list *wq, t_string **out)
{
	t_error err;
	size_t i;

	i = 0;
	if (wq->state == WQS_SINGLY_QUOTED)
		return (NO_ERROR);
	while (wq->part[i])
	{
		if (is_valid_dollar_variable(&wq->part[i]))
		{
			err = expand_dollar_variable(vars, &wq->part[i], out);
			if (err != NO_ERROR)
				return (E_OOM);
			i += 1 + identifier_len(&wq->part[i + 1]);
		}
		else
		{
			if (string_push(out, wq->part[i]) == true)
				return (E_OOM);
			i++;
		}
	}
	return (NO_ERROR);
}

t_error wql_variable_expand(t_expansion_variables vars, t_word_quotes_list *wql)
{
	t_string *expanded;
	char *expanded_c_string; // XXX: this variable name is kinda goofy
	t_error err;

	while (wql)
	{
		expanded = string_new();
		if (!expanded)
			return (E_OOM);
		err = variable_expand_word_quote(vars, wql, &expanded);
		if (err != NO_ERROR)
		{
			string_destroy(expanded);
			return (err);
		}
		err = string_make_c_string(expanded, &expanded_c_string);
		if (err != NO_ERROR)
		{
			string_destroy(expanded);
			return (err);
		}
		free(wql->part);
		wql->part = expanded_c_string;
		string_destroy(expanded);
		wql = wql->next;
	}
	return (err);
}
