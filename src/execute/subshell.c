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

t_launch_result launch_subshell(t_state *state, t_subshell *subshell, t_io io, int fd_to_close) {
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

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("dup2");

	warn_non_empty_redirs(subshell);

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close); // bad (?) may be passed to `execute_command` // bad may err

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
	t_error err = wait_for_process(pid, &exit_status);
	if (err != NO_ERROR)
		return /* kill(pid, SIGKILL), */ (t_command_result){.error = err};

	return (t_command_result){.error = NO_ERROR, .status_code = exit_status};
}
