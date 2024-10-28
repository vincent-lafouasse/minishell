#include "t_io.h"

t_io io_default(void)
{
	return ((t_io){DO_NOT_PIPE, DO_NOT_PIPE});
}

t_io io_new(int input, int output)
{
	return ((t_io){.input = input, .output = output});
}
