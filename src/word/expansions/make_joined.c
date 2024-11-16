#include "libft/string.h"
#include "t_word_quotes_list.h"

#include <stdlib.h>

static size_t wql_cumulated_length(const t_word_quotes_list *wql)
{
	size_t len;

	len = 0;
	while (wql)
	{
		len += ft_strlen(wql->part);
		wql = wql->next;
	}
	return (len);
}

t_error wql_make_joined(const t_word_quotes_list *wql, char **out)
{
	char *joined;
	size_t current_part_len;
	size_t i;

	joined = malloc((wql_cumulated_length(wql) + 1) * sizeof(char));
	if (!joined)
		return (E_OOM);
	i = 0;
	while (wql)
	{
		current_part_len = ft_strlen(wql->part);
		ft_memcpy(joined + i, wql->part, current_part_len);
		i += current_part_len;
		wql = wql->next;
	}
	joined[i] = '\0';
	*out = joined;
	return (NO_ERROR);
}
