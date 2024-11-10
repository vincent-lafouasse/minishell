#include "reduction_internals.h"
#include "../t_command/t_command.h"
#include "../t_symbol/t_symbol.h"

#include <stdbool.h>

#include <assert.h> // temporarily

static t_error reduce_pipeline_rest(t_symbol *pipeline_rest, t_command *out)
{
	t_symbol_array	*productions;
	t_error err;

	productions = pipeline_rest->production;

	if (productions->data[2].production->len == 0)
		return reduce_command(&productions->data[1], out);

	*out = command_new_pipeline((t_command){0}, (t_command){0});
	if (!command_is_initialized(*out))
		return (E_OOM);

	err = reduce_command(&productions->data[1], &out->pipeline->first);
	if (err != NO_ERROR)
		return (err);

	return reduce_pipeline_rest(&productions->data[2], &out->pipeline->second);
}

t_error	reduce_pipeline(t_symbol *pipeline, t_command *out)
{
	t_error err;

	assert (pipeline->kind == SYM_PIPELINE);

	if (pipeline->production->data[1].production->len == 0)
		return reduce_command(&pipeline->production->data[0], out);

	out->pipeline = pipeline_new((t_command){0}, (t_command){0});
	if (!out->pipeline)
		return E_OOM;

	err = reduce_command(&pipeline->production->data[0], &out->pipeline->first);
	if (err != NO_ERROR)
		return (err);
	err = reduce_pipeline_rest(&pipeline->production->data[1], &out->pipeline->second);
	if (err != NO_ERROR)
		return (err);

	*out = command_from_pipeline(out->pipeline);
	return NO_ERROR;
}
