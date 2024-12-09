#include "builtin.h"
#include "../execute.h"
#include "error/t_error.h"
#include "word/t_string/t_string.h"
#include "libft/string.h"
#include "libft/ft_io.h"

#include <assert.h> // bad
#include <stdio.h> // bad
#include <unistd.h>

static bool str_eq(const char* a, const char* b);

t_command_result execute_builtin(t_state *state, t_simple *simple) {
	assert(is_builtin_command(simple));
	assert(simple->words != NULL);


	const char* command = simple->words->contents;

	if (str_eq(command, "echo"))
		return execute_echo(state, simple);
	if (str_eq(command, "cd"))
		return execute_cd(state, simple);
	if (str_eq(command, "pwd"))
		return execute_pwd(state, simple);
	if (str_eq(command, "export"))
		return execute_export(state, simple);
	if (str_eq(command, "unset"))
		return execute_unset(state, simple);
	if (str_eq(command, "env"))
		return execute_env(state, simple);
	if (str_eq(command, "exit"))
		return execute_exit(state, simple);

	return (t_command_result){.error = E_UNREACHABLE};
}

bool is_builtin_command(const t_simple* simple) {
	if (simple->words == NULL)
		return false;

	const char* command = simple->words->contents;

	if (str_eq(command, "echo"))
		return true;
	if (str_eq(command, "cd"))
		return true;
	if (str_eq(command, "pwd"))
		return true;
	if (str_eq(command, "export"))
		return true;
	if (str_eq(command, "unset"))
		return true;
	if (str_eq(command, "env"))
		return true;
	if (str_eq(command, "exit"))
		return true;
	return false;
}

static bool str_eq(const char* a, const char* b) {
	size_t len = ft_strlen(a);
	
	return ft_strncmp(a, b, len + 1) == 0;
}
