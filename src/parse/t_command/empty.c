#include "t_command.h"
#include "libft/stdlib.h"

t_command	command_new_empty(t_command_type type)
{
	void	*alloc;

	if (type == CMD_PIPELINE)
		alloc = ft_calloc(1, sizeof(t_pipeline));
	else if (type == CMD_CONDITIONAL)
		alloc = ft_calloc(1, sizeof(t_conditional));
	else if (type == CMD_SUBSHELL)
		alloc = ft_calloc(1, sizeof(t_subshell));
	else if (type == CMD_SIMPLE)
		alloc = ft_calloc(1, sizeof(t_simple));
	else
		alloc = NULL;
	return (t_command){.type = type, .any = alloc};
}
