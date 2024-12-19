#include "word.h"
#include "libft/string.h"

t_error word_clone(const char *word, char **out)
{
	char *dup;

	dup = ft_strdup(word);
	if (!dup)
		return (E_OOM);
	*out = dup;
	return (NO_ERROR);
}
