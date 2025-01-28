#include "../builtin.h"

#include "execute/execute.h"
#include "error/t_error.h"
#include "libft/string.h"
#include "word/t_string/t_string.h"
#include "word/t_word_list/t_word_list.h"

#include <stdlib.h>
#include <unistd.h>

static t_error append_entry_line(t_env_entry entry, t_string **out)
{
	if (string_extend(out, "export "))
		return (E_OOM);
	if (string_extend(out, entry.key))
		return (E_OOM);
	if (entry.value)
	{
		if (string_extend(out, "=\""))
			return (E_OOM);
		if (string_extend(out, entry.value))
			return (E_OOM);
		if (string_extend(out, "\""))
			return (E_OOM);
	}
	if (string_push(out, '\n'))
		return (E_OOM);
	return (NO_ERROR);
}

static t_error gather_environment(t_env *env, t_string **out)
{
	t_env_entry curr;
	t_error err;

	while (env)
	{
		curr = env->entry;
		err = append_entry_line(curr, out);
		if (err != NO_ERROR)
			return (err);
		env = env->next;
	}
	return (NO_ERROR);
}

static t_command_result write_env_to_stdout(t_string *env)
{
	int status;
	int exit_code;

	status = write(STDOUT_FILENO, &env->data, env->len);
	string_destroy(env);
	if (status < 0)
	{
		report_error("write_env_to_stdout", "write");
		exit_code = EXIT_FAILURE;
	}
	else
		exit_code = EXIT_SUCCESS;
	return command_ok(exit_code);
}

t_command_result print_all_variables(t_state *state)
{
	t_string *env;

	env = string_new();
	if (!env)
		return command_err(E_OOM);
	if (gather_environment(state->env, &env) != NO_ERROR)
	{
		string_destroy(env);
		return command_err(E_OOM);
	}

	return write_env_to_stdout(env);
}
