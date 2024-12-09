#include "execute.h"
#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "execute/t_pid_list/t_pid_list.h"
#include "parse/t_command/t_command.h"
#include "io/t_redir_list/t_redir_list.h"
#include "word/t_word_list/t_word_list.h"
#include "word/expansions/expand.h"
#include "signal/signal.h"
#include "log/log.h" // bad, remove in prod


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

// either simple or subshell or maybe builtin
t_launch_result launch_pipeline_inner(t_state* state, t_command command, t_io io, int fd_to_close) {
	assert(command.type == CMD_SIMPLE || command.type == CMD_SUBSHELL);

	if (command.type == CMD_SIMPLE)
		return launch_simple_command(state, command.simple, io, fd_to_close);
	else // subshell
		return launch_subshell(state, command.subshell, io, fd_to_close);

}

t_launch_result launch_pipeline(t_state *state, t_pipeline *pipeline, t_io ends)
{
	t_command current;
	t_pid_list* pids_to_wait = NULL;

	current = command_from_pipeline(pipeline);
	while (current.type == CMD_PIPELINE)
	{
		pid_t pipe_fd[2];

		pipe(pipe_fd); // bad must check out

		t_io current_io = io_new(ends.input, pipe_fd[WRITE]);
		ends.input = pipe_fd[READ];

		t_launch_result launch_result = launch_pipeline_inner(state, current.pipeline->first,
													  current_io, pipe_fd[READ]);

		io_close(current_io);

		pidl_push_back_link(&pids_to_wait, launch_result.pids); // bad might oom

		current = current.pipeline->second;
	}
	t_launch_result last = launch_pipeline_inner(state, current, ends, CLOSE_NOTHING);
	io_close(ends);

	pidl_push_back_link(&pids_to_wait, last.pids); // bad may oom
	return (t_launch_result){.error = NO_ERROR, .pids = pids_to_wait};
}

t_launch_result launch_simple_command(t_state *state, t_simple *simple, t_io io, int fd_to_close)
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

	if (fd_to_close != CLOSE_NOTHING)
		close(fd_to_close);

	t_expansion_variables vars = (t_expansion_variables){state->env, state->last_status};
	err = variable_expand_words(vars, &simple->words);
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

	// temporarily? where exactly in the code should signal handlers be reset?
	// what happens if we've caught one up until this point? exit(128 + signal)?
	// what about inside of built-in functions?
	reset_signal_handlers();

	char** argv = wl_into_word_array(&simple->words);
	char** envp = env_make_envp(state->env);
	execve(command_path, argv, envp);

	graceful_exit_from_child();
}

t_command_result execute_command(t_state *state, t_command command) {

	t_command_result res;

	if (command.type == CMD_SIMPLE)
	{
		t_launch_result launch_res;
		launch_res = launch_simple_command(state, command.simple, io_default(), CLOSE_NOTHING);
		assert(launch_res.error == NO_ERROR); // bad, should handle launch error gracefully

		int status;
		int options = 0;
		assert(launch_res.pids != NULL);
		waitpid(launch_res.pids->pid, &status, options); // bad, `waitpid` errors should be handled
		res = (t_command_result){.error = NO_ERROR, .status_code = status}; // bad, might err
	}
	else if (command.type == CMD_PIPELINE)
	{
		t_launch_result launch_res;
		launch_res = launch_pipeline(state, command.pipeline, io_default());
		assert(launch_res.error == NO_ERROR); // bad, should handle launch error gracefully

		int status = wait_pipeline(launch_res.pids);
		res = (t_command_result){.error = NO_ERROR, .status_code = status};
	}
	else if (command.type == CMD_CONDITIONAL)
		res = execute_conditional(state, command.conditional);
	else if (command.type == CMD_SUBSHELL)
		res = execute_subshell(state, command.subshell);
	else
		assert(!"unknown command type");

	if (res.error != NO_ERROR)
		printf("err : %d\n",res.error);

	return res;
}
