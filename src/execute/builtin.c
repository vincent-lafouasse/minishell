#include "builtin.h"
#include "libft/string.h"

static bool str_eq(const char* a, const char* b);

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
