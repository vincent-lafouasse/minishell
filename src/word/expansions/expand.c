#include "expand.h"
#include "execute/t_env/t_env.h"
#include "error/t_error.h"
#include "word/t_string/t_string.h"

#include "libft/ctype.h"
#include "libft/ft_string.h"

#include <stdbool.h>

t_error perform_all_expansions_on_words(t_word_list *word)
{
	(void)word;
	return (NO_ERROR); // bad, dummy `perform_all_expansions_on_words` implementation
}

static bool is_identifier(const char* start, size_t len) {
	size_t i = 0;
	
	if (len == 0)
		return false;

	if (ft_isdigit(*start))
		return false;

	while (i < len) {
		if(!ft_isalnum(start[i]) && start[i] != '_')
			return false;
		i++;
	}
	return true;
}

static bool identifier_len(const char *str) {
	size_t len = 0;

	if (ft_isdigit(*str))
		return 0;
	while (ft_isalnum(str[len]) || str[len] == '_') {
		len++;
	}
	return (len);
}

t_error expand_single_word(t_expansion_variables vars, char* word)
{
	bool should_be_expanding;
	size_t i;
	t_string *expanded;

	i = 0;
	should_be_expanding = false;
	expanded = string_new();
	while (word[i]) {
		if (word[i] == '\'')
			should_be_expanding = !should_be_expanding;

		if (should_be_expanding && word[i] == '$') {
			// xxx make sure nothing breaks if variable_name_len = 0
			size_t variable_name_len = identifier_len(word + i + 1);
			char *variable_name = ft_substr(word, i + 1, variable_name_len);
			if (!variable_name)
				return (string_destroy(expanded), E_OOM);
			if (variable_name_len != 0)
			{
				if (word[i + 1] == '?')
					"expand with last_status";
				t_env_entry *variable = env_get(vars.env, variable_name);
			}
		}
		i++;
	}
}
