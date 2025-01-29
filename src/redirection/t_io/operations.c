#include "t_io.h"

#include <unistd.h>

void io_close(t_io io)
{
	if (io.input != DO_NOT_PIPE)
		close(io.input);
	if (io.output != DO_NOT_PIPE)
		close(io.output);
}

t_error do_piping(t_io io)
{
	if (io.input != DO_NOT_PIPE && io.input != STDIN_FILENO)
	{
		if (dup2(io.input, STDIN_FILENO) == -1)
			return E_DUP2;
		close(io.input);
	}
	if (io.output != DO_NOT_PIPE && io.output != STDOUT_FILENO)
	{
		if (dup2(io.output, STDOUT_FILENO) == -1)
			return E_DUP2;
		close(io.output);
	}
	return (NO_ERROR);
}
