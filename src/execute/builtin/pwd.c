#include "builtin.h"

#include "../execute.h"
#include "error/t_error.h"
#include "libft/string.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

static t_error get_current_directory(char **out)
{
	char *buf;

	buf = malloc((PATH_MAX + 1) * sizeof(char));
	if (!buf)
		return (E_OOM);
	if (getcwd(buf, PATH_MAX + 1) == NULL)
	{
		free(buf);
		return (E_GETCWD);
	}
	*out = buf;
	return (NO_ERROR);
}

static int write_cwd_with_newline(char *working_directory)
{
	size_t len;
	char *nul_terminator;
	int write_status;

	len = ft_strlen(working_directory);
	nul_terminator = &working_directory[len];
	*nul_terminator = '\n';
	write_status = write(STDOUT_FILENO, working_directory, len + 1);
	*nul_terminator = '\0';
	return (write_status);
}

t_command_result execute_pwd(t_state *state, t_simple *builtin)
{
	(void)state;
	(void)builtin;
	char *working_directory;
	t_error err;
	int write_status;

	err = get_current_directory(&working_directory);
	if (err == E_OOM)
		return command_err(E_OOM);
	else if (err == E_GETCWD)
	{
		perror("minishell: pwd: getcwd");
		return command_ok(EXIT_FAILURE);
	}
	write_status = write_cwd_with_newline(working_directory);
	free(working_directory);
	if (write_status < 0)
	{
		perror("minishell: pwd: write");
		return command_ok(EXIT_FAILURE);
	}
	return command_ok(EXIT_SUCCESS);
}
