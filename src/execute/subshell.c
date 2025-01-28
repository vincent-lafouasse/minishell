#include "execute.h"
#include "execute/process/process.h"
#include "error/t_error.h"
#include "libft/ft_io.h"
#include "libft/string.h"
#include "log/log.h"
#include "parse/t_command/t_command.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void shell_cleanup(t_state *state); // bad, should be #include "shell.h"

t_error launch_cmd_in_subshell(t_state *state, t_command cmd, t_io io, int fd_to_close) {
	t_error err;
	bool in_child;

	err = fork_and_push_pid(&in_child, &state->our_children);
	if (err != NO_ERROR)
		return err;

	if (!in_child)
		return NO_ERROR;
	else
		pidl_clear(&state->our_children);

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("minishell: do_piping: dup2");

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);

	t_command_result inner_res = execute_command(state, cmd);
	if (inner_res.error != NO_ERROR)
		report_t_error("subshell", err);
	shell_cleanup(state);
	if (inner_res.error != NO_ERROR)
		exit(inner_res.status_code);
	else
		exit(state->last_status);
}

t_error launch_subshell(t_state *state, t_subshell *subshell, t_io io, int fd_to_close) {
	if (subshell->redirections != NULL)
		report_error("warning", "subshell redirections ignored");
	return launch_cmd_in_subshell(state, subshell->cmd, io, fd_to_close);
}

t_command_result execute_subshell(t_state *state, t_subshell *subshell)
{
	t_error err = launch_subshell(state, subshell, io_default(), CLOSE_NOTHING);
	if (err != NO_ERROR) {
		return (t_command_result){.error = err};
	}
	pid_t pid = state->our_children->pid;

	int exit_status;
	err = wait_for_process(state, pid, &exit_status);
	if (err != NO_ERROR)
	{
		exit_status = EXIT_FAILURE;
		perror("minishell: wait_for_process");
	}
	pidl_clear(&state->our_children);
	return (t_command_result){.error = NO_ERROR, .status_code = exit_status};
}
