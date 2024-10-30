#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "libft/ft_string.h"
#include "libft/string.h"

#include <stdlib.h>
#include <unistd.h>

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

typedef struct s_command_properties {
	char	*full_path;
	bool	is_executable;
} t_command_properties;

static t_error find_command_in_path(const char *path, const char *filename, t_command_properties *out)
{
	char *candidate;
	t_command_properties p;
	int x_access;

	candidate = join_delimited(path, '/', filename);
	if (!candidate)
		return (E_OOM);
	x_access = access(candidate, X_OK);
	if (x_access < 0)
	{
		free(candidate);
		candidate = NULL;
		if (errno != ENOENT)
			return (E_ACCESS);
	}
	p.is_executable = x_access == 0;
	p.full_path = candidate;
	*out = p;
	return (NO_ERROR);
}

static t_error find_command_in_path_list(char **path, const char *word, char **out)
{
	size_t i;
	t_error err;
	char *command_filename;
	t_command_properties candidate;

	i = 0;
	command_filename = NULL;
	while (path[i])
	{
		err = find_command_in_path(path[i], word, &candidate);
		if (err != NO_ERROR)
			return err;
		if (candidate.full_path != NULL && candidate.is_executable)
		{
			command_filename = candidate.full_path;
			break;
		}
		free(candidate.full_path);
		i++;
	}
	*out = command_filename;
	return (NO_ERROR);
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

static bool is_absolute_pathname(const char *word)
{
	return (ft_strchr(word, '/') != (char *)NULL);
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
	{
		err = find_command_in_path_list(path, word, out);
		if (err == NO_ERROR && *out == NULL)
			err = E_COMMAND_NOT_FOUND;
	}
	ft_split_destroy(path);
	return (err);
}
