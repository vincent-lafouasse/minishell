#ifndef EXECUTE_H
# define EXECUTE_H

#include "./t_env/t_env.h"
#include "./t_pid_list/t_pid_list.h"
#include "error/t_error.h"
#include "parse/t_command/t_command.h"
#include "io/t_io/t_io.h"

#include <termios.h>

#define CLOSE_NOTHING -1

typedef struct s_state {
	t_env *env;
	t_command root;
	int last_status;
	bool is_interactive;
	char* line;
	bool tty_properties_initialized;
	struct termios tty_properties;
	t_pid_list *our_children;
} t_state;

typedef struct s_command_result {
	t_error error;
	int status_code;
} t_command_result;

static inline t_command_result command_ok(int status_code)
{
	return ((t_command_result){.error = NO_ERROR, .status_code = status_code});
}

static inline t_command_result command_err(t_error err)
{
#define EXIT_FAILURE 1 // bad, replace with EXIT_FAILURE from shell.h
	return ((t_command_result){.error = err, .status_code = EXIT_FAILURE});
}

// internals

t_error launch_pipeline(t_state *state, t_pipeline *pipeline, t_io io);
t_error launch_simple_command(t_state *state, t_simple *simple, t_io io, int fd_to_close);
t_error launch_cmd_in_subshell(t_state *state, t_command cmd, t_io io, int fd_to_close);
t_error launch_subshell(t_state *state, t_subshell *subshell, t_io io, int fd_to_close);

// public

t_command_result execute_command(t_state *state, t_command command);
t_command_result execute_conditional(t_state *state, t_conditional *cond);
t_command_result execute_subshell(t_state *state, t_subshell *subshell);

t_command_result execute_builtin(t_state *state, t_simple *simple);


#endif // EXECUTE_H
