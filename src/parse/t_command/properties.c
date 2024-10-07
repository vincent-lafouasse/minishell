#include "t_command.h"
#include <stddef.h>
#include <stdbool.h>

bool	command_is_initialized(t_command command)
{
	return (command.any != NULL);
}
