#include "builtin.h"

#include "../execute.h"
#include "error/t_error.h"
#include "libft/string.h"
#include "word/t_string/t_string.h"
#include "word/t_word_list/t_word_list.h"

#include <stdlib.h>
#include <unistd.h>

static t_error gather_environment(t_env *env, t_string **out)
{
	t_env_entry curr;

	while (env)
	{
		curr = env->entry;
		if (!curr.value)
		{
			env = env->next;
			continue;
		}
		if (string_extend(out, curr.key))
			return (E_OOM);
		if (string_push(out, '='))
			return (E_OOM);
		if (string_extend(out, curr.value))
			return (E_OOM);
		if (string_push(out, '\n'))
			return (E_OOM);
		env = env->next;
	}
	return (NO_ERROR);
}

t_command_result write_env_to_stdout(t_string *env)
{
	int status;
	int exit_code;

	status = write(STDOUT_FILENO, &env->data, env->len);
	string_destroy(env);
	if (status < 0)
	{
		exit_code = EXIT_FAILURE; // bad, should report write error
	}
	else
		exit_code = EXIT_SUCCESS;
	return (t_command_result){.error = NO_ERROR, .status_code = exit_code};
}

t_command_result execute_env(t_state *state, t_simple *builtin)
{
	t_word_list *arguments;
	t_string *env;

	arguments = builtin->words->next;

	if (arguments != NULL)
	{
		const char *error = "minishell: env: too many arguments\n";
		write(STDERR_FILENO, error, ft_strlen(error));
		return (t_command_result){.error = NO_ERROR, .status_code = EX_BADUSAGE};
	}

	env = string_new();
	if (!env)
		return (t_command_result){.error = E_OOM};
	if (gather_environment(state->env, &env) != NO_ERROR)
	{
		string_destroy(env);
		return (t_command_result){.error = E_OOM};
	}

	return write_env_to_stdout(env);
}
