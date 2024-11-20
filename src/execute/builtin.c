#include "builtin.h"
#include "error/t_error.h"
#include "execute.h"
#include "libft/string.h"

#include <assert.h> // bad
#include <stdio.h> // bad

static bool str_eq(const char* a, const char* b);

t_command_result execute_export(t_state *state, t_simple *builtin)
{
	printf("called export\n");
	return (t_command_result){.error = E_DUMMY};
}
t_command_result execute_echo(t_state *state, t_simple *builtin)
{
	printf("called echo\n");
	return (t_command_result){.error = E_DUMMY};
}

t_command_result execute_cd(t_state *state, t_simple *builtin)
{
	printf("called cd\n");
	return (t_command_result){.error = E_DUMMY};
}
t_command_result execute_unset(t_state *state, t_simple *builtin)
{
	printf("called unset\n");
	return (t_command_result){.error = E_DUMMY};
}
t_command_result execute_env(t_state *state, t_simple *builtin)
{
	printf("called env\n");
	return (t_command_result){.error = E_DUMMY};
}
t_command_result execute_exit(t_state *state, t_simple *builtin)
{
	printf("called exit\n");
	return (t_command_result){.error = E_DUMMY};
}

t_command_result execute_builtin(t_state *state, t_simple *simple) {
	assert(is_builtin_command(simple));
	assert(simple->words != NULL);


	const char* command = simple->words->contents;

	if (str_eq(command, "echo"))
		return execute_echo(state, simple);
	if (str_eq(command, "cd"))
		return execute_cd(state, simple);
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
