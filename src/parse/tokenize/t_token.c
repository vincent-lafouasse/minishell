#include "t_token.h"
#include "libft/string.h"

t_error token_duplicate(t_token token, t_token *out)
{
	char	*literal_copy;

	literal_copy = NULL;
	if (token.literal != NULL)
	{
		literal_copy = ft_strdup(token.literal);
		if (!literal_copy)
			return (E_OOM);
	}
	*out = token;
	out->literal = literal_copy;
	return NO_ERROR;
}
