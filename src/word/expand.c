#include "execute/t_env/t_env.h"
#include "libft/ft_string.h"
#include "libft/string.h"
#include "word.h"

#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

typedef struct s_file_properties {
	bool exists;
	bool is_a_directory;
	bool is_readable;
	bool is_executable;
} t_file_properties;

t_error perform_all_expansions_on_words(t_word_list *word)
{
	(void)word;
	return (NO_ERROR); // bad, dummy `perform_all_expansions_on_words` implementation
}

static void ft_split_destroy(char *data[])
{
	size_t i;

	i = 0;
	while (data[i])
	{
		free(data[i]);
		i++;
	}
	free(data);
}

static char *join_delimited(const char *s1, char delim, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*out;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	out = malloc(len1 + 1 + len2 + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, s1, len1);
	out[len1] = delim;
	ft_memcpy(out + len1 + 1, s2, len2);
	out[len1 + 1 + len2] = '\0';
	return (out);
}

static t_file_properties get_file_properties(const char *path)
{
	struct stat stat_info;
	t_file_properties p;

	p = (t_file_properties){.exists = false, 0};
	if (stat(path, &stat_info) < 0)
		return (p);
	p.exists = true;
	if (!S_ISDIR(stat_info.st_mode))
	{
		p.is_executable = access(path, X_OK) == 0;
		p.is_readable = access(path, R_OK) == 0;
	}
	else
		p.is_a_directory = true;
	return (p);
}

static t_error find_command_in_path_list(char **path, const char *word, char **out)
{
	size_t i;
	char *candidate;
	t_file_properties properties;

	i = 0;
	*out = NULL;
	while (path[i])
	{
		candidate = join_delimited(path[i], '/', word);
		if (candidate == NULL)
			return E_OOM;
		properties = get_file_properties(candidate);
		if (properties.exists && properties.is_executable)
		{
			*out = candidate;
			return (NO_ERROR);
		}
		free(candidate);
		i++;
	}
	return (E_COMMAND_NOT_FOUND);
}

static bool is_absolute_pathname(const char *word)
{
	return (ft_strchr(word, '/') != (char *)NULL);
}

static t_error copy_word(const char *word, char **out)
{
	char *dup;

	dup = ft_strdup(word);
	if (!dup)
		return (E_OOM);
	*out = dup;
	return (NO_ERROR);
}

t_error path_expanded_word(const t_env *env, const char *word, char **out)
{
	char **path;
	t_error err;

	path = env_make_path_or_empty(env); // bad, empty path entries should map to "." and `ft_split` ignores them
	if (!path)
		return (E_OOM);
	if (is_absolute_pathname(word))
		err = copy_word(word, out);
	else
		err = find_command_in_path_list(path, word, out);
	ft_split_destroy(path);
	return (err);
}
