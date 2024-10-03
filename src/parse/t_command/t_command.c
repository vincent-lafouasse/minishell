#include "t_command.h"
#include "libft/stdlib.h"
#include <stdbool.h>
#include <stdlib.h>

t_command	command_new(t_command_type type)
{
	void	*alloc;

	if (type == PIPELINE_CMD)
		alloc = ft_calloc(1, sizeof(t_pipeline));
	else if (type == CONDITIONAL_CMD)
		alloc = ft_calloc(1, sizeof(t_conditional));
	else if (type == SUBSHELL_CMD)
		alloc = ft_calloc(1, sizeof(t_subshell));
	else if (type == SIMPLE_CMD)
		alloc = ft_calloc(1, sizeof(t_simple));
	else
		alloc = NULL;
	return (t_command){.type = type, .any = alloc};
}

bool	command_is_initialized(t_command command)
{
	return (command.any != NULL);
}
