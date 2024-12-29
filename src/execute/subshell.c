#include "execute.h"
#include "execute/process/process.h"
#include "error/t_error.h"
#include "libft/string.h"
#include "parse/t_command/t_command.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

static void warn_non_empty_redirs(const t_subshell* s) {
	const char* msg = "minishell: warning: ignored redirs in subshell\n";
	if (s->redirections != NULL)
		write(STDERR_FILENO, msg, ft_strlen(msg));
}

t_launch_result launch_cmd_in_subshell(t_state *state, t_command cmd, t_io io, int fd_to_close) {
	t_error err;
	t_pid_list* pids = NULL;
	bool in_child;

	err = fork_and_push_pid(&in_child, &pids);
	if (err != NO_ERROR)
		return (t_launch_result){.error = err, .pids = NULL};

	if (!in_child)
		return (t_launch_result){.error = NO_ERROR, .pids = pids};

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("minishell: do_piping: dup2");

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);

	t_command_result inner_res = execute_command(state, cmd); // bad?, log err ?
	exit(inner_res.status_code); // bad. dont know what status to return yet
}

t_launch_result launch_subshell(t_state *state, t_subshell *subshell, t_io io, int fd_to_close) {
	t_error err;
	t_pid_list* pids = NULL;
	bool in_child;

	err = fork_and_push_pid(&in_child, &pids);
	if (err != NO_ERROR)
		return (t_launch_result){.error = err, .pids = NULL};

	if (!in_child)
		return (t_launch_result){.error = NO_ERROR, .pids = pids};

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("minishell: do_piping: dup2");

	warn_non_empty_redirs(subshell);

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);

	t_command_result inner_res = execute_command(state, subshell->cmd); // bad?, log err ?
	exit(inner_res.status_code); // bad. dont know what status to return yet
}

t_command_result execute_subshell(t_state *state, t_subshell *subshell)
{
	t_launch_result launch_result = launch_subshell(state, subshell, io_default(), CLOSE_NOTHING);
	if (launch_result.error != NO_ERROR) {
		return (t_command_result){.error = launch_result.error};
	}
	pid_t pid = launch_result.pids->pid;

	int exit_status;
	t_error err = wait_for_process(state, pid, &exit_status);
	if (err != NO_ERROR)
	{
		exit_status = EXIT_FAILURE;
		perror("minishell: wait_for_process");
	}
	return (t_command_result){.error = NO_ERROR, .status_code = exit_status};
}
