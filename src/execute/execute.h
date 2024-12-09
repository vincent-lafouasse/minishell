#ifndef EXECUTE_H
# define EXECUTE_H

#include "./t_env/t_env.h"
#include "./t_pid_list/t_pid_list.h"
#include "error/t_error.h"
#include "parse/t_command/t_command.h"
#include "io/t_io/t_io.h"

#define CLOSE_NOTHING -1

#define EXIT_FAILED_REDIRECT 259

typedef struct s_state {
	t_env *env;
	t_command root;
	int last_status;
	char* line;
} t_state;

typedef struct s_launch_result {
	t_error error;
	t_pid_list* pids;
} t_launch_result;

typedef struct s_command_result {
	t_error error;
	int status_code;
} t_command_result;

// internals

t_launch_result launch_pipeline(t_state *state, t_pipeline *pipeline, t_io io);
t_launch_result launch_simple_command(t_state *state, t_simple *simple, t_io io, int fd_to_close);
t_launch_result launch_subshell(t_state *state, t_subshell *subshell, t_io io, int fd_to_close);

// public

t_command_result execute_command(t_state *state, t_command command);
t_command_result execute_conditional(t_state *state, t_conditional *cond);
t_command_result execute_subshell(t_state *state, t_subshell *subshell);
int wait_pipeline(t_pid_list* pids); // bad, should handle EINTR


t_command_result execute_builtin(t_state *state, t_simple *simple);


#endif // EXECUTE_H
