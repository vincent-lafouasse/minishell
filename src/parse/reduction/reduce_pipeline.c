#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <stdbool.h>

#include <assert.h> // temporarily

static bool recurse(t_command *out, t_symbol *pipeline_rest)
{
	t_symbol_array	*productions;
	t_pipeline		*pipeline;
	t_command		first;
	t_command		second;

	productions = pipeline_rest->production;

	if (productions->len == 0)
		return (false);

	first = reduce_command(&productions->data[1]);
	if (!recurse(&second, &productions->data[2]))
	{
		*out = first;
		return (true);
	}

	pipeline = pipeline_new(first, second);
	assert (pipeline != NULL);

	*out = command_from_pipeline(pipeline);

	return true;
}

static t_command	reduce_pipeline_rest(t_symbol *pipeline_rest)
{
	t_command out;

	assert (pipeline_rest->kind == PIPELINE_REST);
	assert (pipeline_rest->production->len > 0);

	recurse(&out, pipeline_rest);

	return out;
}

t_command	reduce_pipeline(t_symbol *pipeline)
{
	t_pipeline	*out;

	assert (pipeline->kind == PIPELINE);

	if (pipeline->production->data[1].production->len == 0)
		return reduce_command(&pipeline->production->data[0]);

	out = pipeline_new((t_command){0}, (t_command){0});
	assert (out != NULL);

	out->first = reduce_command(&pipeline->production->data[0]);
	out->second = reduce_pipeline_rest(&pipeline->production->data[1]);

	return command_from_pipeline(out);
}
