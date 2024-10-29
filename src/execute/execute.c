#include "execute.h"
#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "execute/t_fd_list/t_fd_list.h"
#include "execute/t_pid_list/t_pid_list.h"
#include "parse/t_command/t_command.h"
#include "io/t_redir_list/t_redir_list.h"
#include "word/t_word_list/t_word_list.h"
#include "word/word.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h> // bad

#define READ 0
#define WRITE 1

// ? perform expansion on all words -> malloc
// ? expand_path on first word -> malloc,access
// ? generate argv, envp -> malloc
// ? fork
//  ---->   parent
//   |      return
//   |
//   |----> child
//           ? apply t_io redirections -> dup2
//		     ? apply all simple->redirections -> open,dup2 NOT close
//		     ? execute command execve
//           ---->   fails
//            |      unwind all local allocations
//            |      close local fds
//            |      signal `must_exit`
//            |
//            |----> succeeds

_Noreturn
static void graceful_exit_from_child() // bad dummy
{
	exit(EXIT_FAILURE); // bad, should clean up all allocations before exiting from child process
}

int wait_pipeline(t_pid_list* pids) // bad, should handle EINTR
{
	t_pid_list* current = pids;
	int status;

	while (current)
	{
		waitpid(current->pid, &status, 0); // hack, should not wait sequentially?
		current = current->next;
	}
	return (status);
}

t_launch_result launch_pipeline(t_state *state, t_pipeline *pipeline, t_io ends, t_pid_list** pids)
{
	t_command current;

	current = command_from_pipeline(pipeline);
	while (current.type == PIPELINE_CMD)
	{
		pid_t pipe_fd[2];

		assert (current.pipeline->first.type == SIMPLE_CMD);

		pipe(pipe_fd); // bad must check out


		t_fd_list *fds_to_close = NULL;
		if (fdl_push_front(&fds_to_close, pipe_fd[READ]) == E_OOM)
			abort(); // bad

		t_io current_io = (t_io){ends.input, pipe_fd[WRITE]};
		ends.input = pipe_fd[READ];

		t_launch_result launch_result = launch_simple_command(state, current.pipeline->first.simple,
													  current_io, &fds_to_close);

		io_close(current_io);

		pidl_push_back_link(pids, launch_result.pids);

		fdl_clear(&fds_to_close);
		current = current.pipeline->second;
	}
	t_launch_result last = launch_simple_command(state, current.simple, ends, NULL);
	io_close(ends);

	pidl_push_back_link(pids, last.pids);

	return (t_launch_result){.error = NO_ERROR}; // bad dummy
}

t_launch_result launch_simple_command(t_state *state, t_simple *simple, t_io io, t_fd_list **fds_to_close)
{
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

	fdl_close_and_clear(fds_to_close);

	err = perform_all_expansions_on_words(simple->words);
	if (err != NO_ERROR)
		graceful_exit_from_child();

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("dup2");

	err = apply_redirections(simple->redirections);
	if (err != NO_ERROR)
		graceful_exit_from_child();

	char *command_path;
	err = path_expanded_word(state->env, simple->words->contents, &command_path);
	if (err != NO_ERROR)
		graceful_exit_from_child(); // bad, should exit with status 127 or 126
									// if the command could not be found or if
									// we don't have execution permissions to
									// the candiate executable, respectively

	char** argv = wl_into_word_array(&simple->words);
	char** envp = env_make_envp(state->env);
	execve(command_path, argv, envp);

	graceful_exit_from_child();
}
