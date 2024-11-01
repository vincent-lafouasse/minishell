#include "execute.h"
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

t_launch_result launch_subshell(t_state *state, t_subshell *subshell, t_io io, t_fd_list **fds_to_close) {
	warn_non_empty_redirs(subshell);
	t_error err;
	t_pid_list* pids = pidl_new(0);
	if (pids == NULL)
	{
		free(pids);
		return (t_launch_result){.error = E_OOM, .pids = NULL};
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		free(pids);
		return (t_launch_result){.error = E_FORK, .pids = NULL};
	}
	if (pid != 0)
	{
		pids->pid = pid;
		return (t_launch_result){.error = NO_ERROR, .pids = pids};
	}

	t_command inner = subshell->cmd;
	assert (inner.type == SIMPLE_CMD);

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("dup2");

	t_launch_result launch_res;
	if (inner.type == SIMPLE_CMD)
		launch_res = launch_simple_command(state, inner.simple, io, fds_to_close);
	else if (inner.type == PIPELINE_CMD)
		launch_res = launch_pipeline(state, inner.simple, io, fds_to_close);
	else {
		launch_res = (t_launch_result){};
	}
	

	return launch_res;
}

t_command_result execute_subshell(t_state *state, t_subshell *subshell)
{
	warn_non_empty_redirs(subshell);
	t_command inner = subshell->cmd;
	assert (inner.type == SIMPLE_CMD);

	t_launch_result launch_result = launch_subshell(state, subshell, io_default(), NULL);
	if (launch_result.error != NO_ERROR) {
		return (t_command_result){.error = launch_result.error};
	}
	pid_t to_wait = launch_result.pids->pid;

	int status;
	int options = 0;
	waitpid(to_wait, &status, options);

	return (t_command_result){.error = NO_ERROR, .status_code = status};
}