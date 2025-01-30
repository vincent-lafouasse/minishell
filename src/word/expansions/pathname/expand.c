#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "word/word.h"
#include "libft/string.h"

#include <stdlib.h>
#include <unistd.h>

char *join_delimited(const char *s1, char delim, const char *s2);
void ft_split_destroy(char *data[]);
bool is_absolute_pathname(const char *word);
bool filename_is_directory(const char *command_path);

typedef struct s_command_properties {
	char	*full_path;
	bool	is_executable;
} t_command_properties;

static t_error find_command_in_path(const char *path, const char *filename, t_command_properties *out)
{
	char *candidate;
	t_command_properties p;

	candidate = join_delimited(path, '/', filename);
	if (!candidate)
		return (E_OOM);
	p = (t_command_properties){0};
	p.is_executable = (access(candidate, X_OK) == 0) && !filename_is_directory(candidate);
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

t_error path_expanded_word(const t_env *env, const char *word, char **out)
{
	char **path;
	t_error err;

	path = env_make_path_or_empty(env);
	if (!path)
		return (E_OOM);
	if (is_absolute_pathname(word) || path[0] == NULL)
		err = word_clone(word, out);
	else
	{
		err = find_command_in_path_list(path, word, out);
		if (err == NO_ERROR && *out == NULL)
			err = E_COMMAND_NOT_FOUND;
	}
	ft_split_destroy(path);
	return (err);
}
