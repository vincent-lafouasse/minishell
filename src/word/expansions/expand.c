#include "expand.h"
#include "execute/t_env/t_env.h"

#include "libft/ctype.h"

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

t_error perform_all_expansions_on_words(t_word_list *word)
{
	(void)word;
	return (NO_ERROR); // bad, dummy `perform_all_expansions_on_words` implementation
}

enum e_quote_context {NOT_IN_QUOTE, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE};

t_error expand_single_word(char* filename, t_state* state)
{
	enum e_quote_context quote_ctx = NOT_IN_QUOTE;

	size_t i = 0;
	while (filename[i]) {
		if (filename[i] == '\'') {
			while (filename[i] && filename[i] != '\'')
				i++;
		}

		if (filename[i] == '"') {
			if (quote_ctx == NOT_IN_QUOTE) {
				quote_ctx = IN_DOUBLE_QUOTE;
			} else if (quote_ctx == IN_DOUBLE_QUOTE) {
				quote_ctx = NOT_IN_QUOTE;
			}
		}

		if (filename[i] == '$') {
			// xxx make sure nothing breaks if variable_name_len = 0
			size_t variable_name_len = identifier_len(filename + i);
			if (variable_name_len != 0)
			{
				if (filename[i + 1] == '?')
					"expand with last_status";
				t_env_entry variable = env_get(state->env, xxx);
			}
		}
		i++;
	}
}
