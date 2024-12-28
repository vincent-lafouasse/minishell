#include "./expand.h"
#include "./expand_internals.h"

#include "word/t_string/t_string.h"
#include "execute/t_env/t_env.h"
#include "error/t_error.h"

#include "libft/string.h"
#include "libft/ctype.h"
#include "libft/ft_string.h"

#include <stdlib.h>

static size_t variable_name_len(const char *var_name);

static t_error append_variable_value(t_expansion_variables vars, \
									  const char *var_name, t_string **out)
{
	char *var_name_substr;
	const t_env_entry *var_entry;
	char *var_value;
	t_error err;

	var_name_substr = ft_substr(var_name, 0, variable_name_len(var_name));
	if (!var_name_substr)
		return (E_OOM);
	if (*var_name == '?')
		var_value = ft_itoa(vars.last_status);
	else
	{
		var_entry = env_get(vars.env, var_name_substr);
		if (var_entry && var_entry->value)
			var_value = ft_strdup(var_entry->value);
		else
			var_value = ft_strdup("");
	}
	if (!var_value)
		return (free(var_name_substr), E_OOM);
	err = string_extend(out, var_value);
	free(var_name_substr);
	free(var_value);
	return (err);
}

static size_t variable_name_len(const char *var_name)
{
	size_t len;

	len = 0;
	if (*var_name == '?')
		return 1;
	if (ft_isdigit(*var_name))
		return 0;
	while (ft_isalnum(var_name[len]) || var_name[len] == '_')
		len++;
	return (len);
}

static bool is_valid_dollar_variable(const char *start)
{
	const char *variable_name;
	bool is_special_variable_name;

	if (*start != '$')
		return (false);
	variable_name = start + 1;
	is_special_variable_name = *variable_name == '?';
	return (is_special_variable_name || variable_name_len(variable_name) > 0);
}

static t_error variable_expand_word_quote(t_expansion_variables vars, \
										  t_word_quotes_list *wq, t_string **out)
{
	t_error err;
	size_t i;
	char *variable_name_start;

	i = 0;
	while (wq->part[i])
	{
		if (wq->state != WQS_SINGLY_QUOTED && \
			is_valid_dollar_variable(&wq->part[i]))
		{
			variable_name_start = &wq->part[i + 1];
			err = append_variable_value(vars, variable_name_start, out);
			if (err != NO_ERROR)
				return (err);
			i += 1 + variable_name_len(variable_name_start);
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
	char *expanded_c_string;
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
