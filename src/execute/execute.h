#ifndef EXECUTE_H
# define EXECUTE_H

#include "./t_env/t_env.h"
#include "./t_pid_list/t_pid_list.h"
#include "error/t_error.h"
#include "parse/t_command/t_command.h"


#define DO_NOT_PIPE -1
#define NO_WAIT 0

typedef struct s_io {
	int input;
	int output;
} t_io;

t_io io_default(void);
t_io io_new(int input, int output);

typedef struct s_state {
	t_env *env;
	t_command root;
	int last_status;
	char* line;
} t_state;

typedef struct s_command_result {
	t_error error;
	bool must_exit;
	t_pid_list* pids_out;
} t_command_result;

// internals

t_command_result execute_simple_command(t_state *state, t_simple *simple, t_io io);
t_command_result execute_pipeline(t_state *state, t_pipeline *pipeline, t_io io);

// public

t_command_result execute_command(t_state *state, t_command command);

#endif // EXECUTE_H
