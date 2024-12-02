#include "builtin.h"

#include "../execute.h"

#include "error/t_error.h"
#include "libft/ctype.h"
#include "libft/ft_string.h"
#include "libft/string.h"
#include "execute/t_env/t_env.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

typedef struct s_assignment
{
	bool appending;
	char *name;
	char *value;
} t_assignment;

static void report_invalid_identifier(char *identifier_name)
{
	(void)identifier_name;
	const char *message = "minishell: export: invalid identifier\n";
	write(STDERR_FILENO, message, ft_strlen(message));
}

// returns 0 if assignment is malformated in any way
static size_t name_len(char *assignment)
{
	size_t i;

	if (!ft_isalpha(*assignment) && *assignment != '_')
		return (0);
	i = 1;
	while (assignment[i])
	{
		if (assignment[i] == '=')
			return (i);
		if (assignment[i] == '+' && assignment[i + 1] == '=')
			return (i);
		if (!ft_isalnum(assignment[i]) && assignment[i] != '_')
			return (0);
		i++;
	}
	return (i);
}

static t_error parse_assignment(char *str, t_assignment *out)
{
	size_t len;

	*out = (t_assignment) {0};
	len = name_len(str);
	if (len == 0)
		return NO_ERROR;
	out->name = ft_substr(str, 0, len);
	if (!out->name)
		return (E_OOM);
	if (str[len] == '\0')
	{
		out->value = NULL;
		out->appending = false;
	}
	else if (str[len] == '+')
	{
		out->value = ft_strdup(&str[len + 2]);
		if (!out->value)
		{
			free(out->name);
			out->name = NULL;
			return (E_OOM);
		}
		out->appending = true;
	}
	else if (str[len] == '=')
	{
		out->value = ft_strdup(&str[len + 1]);
		if (!out->value)
		{
			free(out->name);
			out->name = NULL;
			return (E_OOM);
		}
		out->appending = false;
	}
	return NO_ERROR;
}

#include <stdio.h>

static t_error assign_variable(t_env *env, t_assignment assignment)
{
	(void)env;
	printf("assignment: \n");
	printf(" > appending: %d\n", assignment.appending);
	printf(" > name: %s\n", assignment.name);
	printf(" > value: %s\n", assignment.value);
	return (NO_ERROR);
}

static t_command_result do_assignments(t_env *env, t_word_list *assignments)
{
	bool any_failed;
	t_assignment assignment;
	t_error err;

	while (assignments)
	{
		err = parse_assignment(assignments->contents, &assignment);
		if (err != NO_ERROR)
			return (t_command_result){.error = err};
		if (!assignment.name)
		{
			any_failed = true;
			report_invalid_identifier(assignments->contents);
			assignments = assignments->next;
			continue;
		}
		err = assign_variable(env, assignment);
		if (err != NO_ERROR)
		{
			free(assignment.name);
			free(assignment.value);
			return (t_command_result){.error = err};
		}
		assignments = assignments->next;
	}
	return (t_command_result) {.error = NO_ERROR, .status_code = any_failed ?
								EXIT_FAILURE : EXIT_SUCCESS};
}

t_command_result print_all_variables(t_state *state)
{
	(void)state;
	printf("print all variables\n");
	return (t_command_result) {0};
}

t_command_result execute_export(t_state *state, t_simple *builtin)
{
	t_word_list *assignments;

	assignments = builtin->words->next;

	if (assignments)
		return do_assignments(state->env, assignments);
	else
		return print_all_variables(state);
}
