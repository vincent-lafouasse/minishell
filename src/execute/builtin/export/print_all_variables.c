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

t_command_result print_all_variables(t_state *state)
{
	t_string *env;

	env = string_new();
	if (!env)
		return (t_command_result){.error = E_OOM};
	if (gather_environment(state->env, &env) != NO_ERROR)
	{
		string_destroy(env);
		return (t_command_result){.error = E_OOM};
	}

	write(STDOUT_FILENO, &env->data, env->len); // maybe bad, write error may be handled
	string_destroy(env);
	return (t_command_result){.error = NO_ERROR, .status_code = 0};
}

