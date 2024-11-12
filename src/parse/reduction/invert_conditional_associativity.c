#include "libft/stdlib.h"
#include "reduction_internals.h"
#include <assert.h> // temporarily
#include <stdlib.h>

typedef struct s_cond_data
{
	t_command				*commands;
	t_conditional_operator	*operators;
	size_t					connector_count;
}							t_cond_data;

static t_cond_data			gather_data_and_free(t_conditional *cond);
static t_error				reconstruct_conditional_command(t_cond_data data,
								t_conditional **out);
static t_cond_data			cond_data_allocate(size_t connector_count);
static size_t				n_connectors(const t_conditional *cond);
static void					destroy_cond_data(t_cond_data data, size_t start) {
	for (size_t i = start; i < data.connector_count; i++) {
		command_destroy(data.commands[i]);
	}
}

t_error	invert_conditional_associativity(t_conditional **out)
{
	t_cond_data	data;
	t_error err;

	data = gather_data_and_free(*out);
	if (data.connector_count == 0)
		return (E_OOM);
	err = reconstruct_conditional_command(data, out);
	free(data.commands);
	free(data.operators);
	return (err);
}

static t_cond_data	gather_data_and_free(t_conditional *cond)
{
	size_t			n;
	t_cond_data		data;
	size_t			i;
	t_conditional	*next;

	n = n_connectors(cond);
	data = cond_data_allocate(n);
	if (data.connector_count == 0)
		return ((t_cond_data){0});
	i = 0;
	while (cond->second.type == CMD_CONDITIONAL)
	{
		data.commands[i] = cond->first;
		data.operators[i] = cond->op;
		next = cond->second.conditional;
		free(cond);
		cond = next;
		i++;
	}
	data.commands[i] = cond->first;
	data.operators[i] = cond->op;
	data.commands[i + 1] = cond->second;
	free(cond);
	return (data);
}

static t_error	reconstruct_conditional_command(t_cond_data data,
		t_conditional **out)
{
	t_conditional	*root;
	size_t			i;
	t_command		lhs;
	t_conditional	*new_root;

	root = conditional_new(data.operators[0], data.commands[0],
			data.commands[1]);
	if (!root)
		return (destroy_cond_data(data, 0), E_OOM);
	i = 1;
	while (i < data.connector_count)
	{
		lhs = command_from_conditional(root);
		new_root = conditional_new(data.operators[i], lhs, data.commands[i
				+ 1]);
		if (!new_root)
		{
			*out = root;
			return (destroy_cond_data(data, i), E_OOM);
		}
		root = new_root;
		++i;
	}
	*out = root;
	return (NO_ERROR);
}

static t_cond_data	cond_data_allocate(size_t connector_count)
{
	t_command				*commands;
	t_conditional_operator	*operators;

	commands = ft_calloc(connector_count + 1, sizeof(*commands));
	operators = ft_calloc(connector_count, sizeof(*operators));
	if (commands == NULL || operators == NULL)
	{
		free(commands);
		free(operators);
		return ((t_cond_data){0});
	}
	return ((t_cond_data){.commands = commands, .operators = operators,
		.connector_count = connector_count});
}

static size_t	n_connectors(const t_conditional *cond)
{
	size_t	n;

	n = 1;
	while (cond->second.type == CMD_CONDITIONAL)
	{
		cond = cond->second.conditional;
		n++;
	}
	return (n);
}
