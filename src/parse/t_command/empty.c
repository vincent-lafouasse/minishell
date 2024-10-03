#include "t_command.h"
#include "libft/stdlib.h"

t_command	command_new_empty(t_command_type type)
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
