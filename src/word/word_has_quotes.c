#include "word.h"
#include "libft/string.h"
#include <stdbool.h>

bool word_has_quotes(const char *word)
{
	char *double_quote;
	char *single_quote;

	double_quote = ft_strchr(word, '"');
	single_quote = ft_strchr(word, '\'');
	return (double_quote || single_quote);
}
