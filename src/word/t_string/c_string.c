#include "libft/string.h"
#include "t_string.h"

#include <stdlib.h>

t_error		string_make_c_string(const t_string *self, char **out)
{
	char *new;
	size_t len;

	len = self->len;
	new = malloc((len + 1) * sizeof(char));
	if (!new)
		return (E_OOM);
	ft_memcpy(new, &self->data, len);
	new[len] = 0;
	*out = new;
	return (NO_ERROR);
}
