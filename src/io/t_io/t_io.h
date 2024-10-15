#ifndef T_IO_H
# define T_IO_H

# include "error/t_error.h"

#define DO_NOT_PIPE -1
#define NO_WAIT 0

typedef struct s_io {
	int input;
	int output;
} t_io;

t_io io_default(void);
t_io io_new(int input, int output);

void io_close(t_io io);
t_error do_piping(t_io io);

#endif // T_IO_H
