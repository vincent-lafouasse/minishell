#include "reduction_internals.h"
#include <assert.h> // temporarily
#include "libft/stdlib.h"
#include <stdlib.h>

typedef struct s_conditional_cmd_data
{
	t_command					*commands;
	t_conditional_operator		*operators;
	size_t						n;
}								t_conditional_cmd_data;

static size_t	n_connectors(const t_conditional *cond);
static t_conditional_cmd_data	cond_data_allocate(size_t n);
static t_conditional_cmd_data	gather_data_and_free(t_conditional *cond);
static t_error	reconstruct_conditional_command(t_conditional_cmd_data data,
		t_conditional **out);


t_error	invert_conditional_associativity(t_conditional **out)
{
	t_conditional_cmd_data	data;

	data = gather_data_and_free(*out);
	*out = NULL;
	if (data.n == 0)
		return (E_OOM);
	return (reconstruct_conditional_command(data, out));
}

static t_conditional_cmd_data	gather_data_and_free(t_conditional *cond)
{
	t_conditional_cmd_data	data;
	t_conditional			*next;
	size_t					n;
	size_t					i;

	n = n_connectors(cond);
	data = cond_data_allocate(n);
	if (data.n == 0)
		return ((t_conditional_cmd_data){0});
	i = 0;
	while (cond->second.type == CONDITIONAL_CMD)
	{
		data.commands[i] = cond->first;
		data.operators[i] = cond->op;
		i++;
		next = cond->second.conditional;
		free(cond);
		cond = next;
	}
	data.commands[i] = cond->first;
	data.operators[i] = cond->op;
	data.commands[i + 1] = cond->second;
	free(cond);
	return (data);
}

static t_error	reconstruct_conditional_command(t_conditional_cmd_data data,
		t_conditional **out)
{
	t_conditional	*root;
	t_conditional	*new_root;
	t_command		lhs;
	size_t			i;

	root = conditional_new(data.operators[0], data.commands[0],
			data.commands[1]);
	if (!root)
		return (E_OOM);
	i = 0;
	while (i < data.n)
	{
		lhs = command_from_conditional(root);
		new_root = conditional_new(data.operators[i], lhs, data.commands[i
				+ 1]);
		if (!new_root)
		{
			*out = root;
			return (E_OOM);
		}
		root = new_root;
		++i;
	}
	*out = root;
	return (NO_ERROR);
}

static t_conditional_cmd_data	cond_data_allocate(size_t n)
{
	t_command				*commands;
	t_conditional_operator	*operators;

	commands = ft_calloc(n + 1, sizeof(*commands));
	operators = ft_calloc(n, sizeof(*operators));
	if (commands == NULL || operators == NULL)
	{
		free(commands);
		free(operators);
		return ((t_conditional_cmd_data){0});
	}
	return ((t_conditional_cmd_data){.commands = commands,
		.operators = operators, .n = n});
}

static size_t	n_connectors(const t_conditional *cond)
{
	size_t	n;

	n = 1;
	while (cond->second.type == CONDITIONAL_CMD)
	{
		cond = cond->second.conditional;
		n++;
	}
	return (n);
}
