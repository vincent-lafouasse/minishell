#include "expand.h"
#include "execute/t_env/t_env.h"
#include "error/t_error.h"
#include "libft/string.h"
#include "word/t_string/t_string.h"

#include "libft/ctype.h"
#include "libft/ft_string.h"

#include <stdlib.h>
#include <stdbool.h>

t_error perform_all_expansions_on_words(t_word_list *word)
{
	(void)word;
	return (NO_ERROR); // bad, dummy `perform_all_expansions_on_words` implementation
}

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

t_error variable_expand_string(t_expansion_variables vars, const char *str, char **out)
{
	bool should_be_expanding;
	size_t i;
	t_string *expanded;
	t_error err;

	i = 0;
	should_be_expanding = true;
	expanded = string_new();
	while (str[i]) {
		if (str[i] == '\'')
			should_be_expanding = !should_be_expanding;
		if (should_be_expanding && is_valid_dollar_variable(&str[i]))
		{
			err = expand_dollar_variable(vars, &str[i], &expanded);
			if (err != NO_ERROR)
				return (string_destroy(expanded), E_OOM);
			i += 1 + identifier_len(&str[i]);
		}
		else
		{
			if (string_push(&expanded, str[i]) == true)
				return (string_destroy(expanded), E_OOM);
			i++;
		}
	}
	err = string_make_c_string(expanded, out);
	string_destroy(expanded);
	return (err);
}
