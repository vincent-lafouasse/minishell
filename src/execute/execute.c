#include "execute.h"
#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "execute/t_pid_list/t_pid_list.h"
#include "parse/t_command/t_command.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include "word/t_word_list/t_word_list.h"
#include "word/word.h"

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
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

static t_error do_piping(t_io io)
{
	if (io.input != DO_NOT_PIPE && io.input != STDIN_FILENO)
	{
		if (dup2(io.input, STDIN_FILENO) == -1)
			return E_DUP2;
		close(io.input);
	}
	if (io.output != DO_NOT_PIPE && io.output != STDOUT_FILENO)
	{
		if (dup2(io.output, STDOUT_FILENO) == -1)
			return E_DUP2;
		close(io.output);
	}
	return (NO_ERROR);
}

int open_flags_for_redir_kind(t_redir_kind kind)
{
	if (kind == FROM_FILE)
		return O_RDONLY;
	else if (kind == INTO_FILE)
		return O_CREAT | O_RDWR | O_TRUNC;
	else if (kind == APPEND_INTO_FILE)
		return O_CREAT | O_APPEND | O_RDWR;
	else
		return -1;
}

static t_error apply_redirections(t_redir_list *redirections, int **fds_to_close)
{
	t_redirect redir;
	int fd;

	/*
	*fds_to_close = ft_calloc(redir_count + 1, sizeof(int));
	if (!*fds_to_close)
		return E_OOM;
	*fds_to_close[redir_count] = -1;
	*/
	while (redirections)
	{
		redir = redirections->redirect;

		if (redir.kind == HERE_DOCUMENT)
			return (E_DUMMY);

		/*
		char *expanded_filename;
		err = expand(redir.filename, &expanded_filename);
		if (AMBIGUOUS_REDIRECT)
			handle();
		*/

		fd = open(redir.filename, O_CLOEXEC | open_flags_for_redir_kind(redir.kind), 0666);
		if (fd == -1)
			return E_DUMMY; // bad, should short circuit command evaluation if file does not exist
		if (redir.kind == FROM_FILE || redir.kind == HERE_DOCUMENT)
			dup2(fd, STDIN_FILENO); // bad, should report dup2 error
		else if (redir.kind == INTO_FILE || redir.kind == APPEND_INTO_FILE)
			dup2(fd, STDOUT_FILENO); // bad, should report dup2 error

	/*
		*fds_to_close[i] = fd;

	*/
		redirections = redirections->next;
	}
	return (NO_ERROR);
}

_Noreturn
static void graceful_exit_from_child() // bad dummy
{
	exit(EXIT_FAILURE); // bad, should clean up all allocations before exiting from child process
}

void wait_for_all_pids(t_pid_list* pids) // dummy
{
	t_pid_list* current = pids;

	while (current)
	{
		int status;
		waitpid(current->pid, &status, 0);
		current = current->next;
	}
}

t_command_result execute_pipeline(t_state *state, t_pipeline *pipeline, t_io io, t_pid_list** pids)
{
	t_command first = pipeline->first;
	t_command second = pipeline->second;

	assert (first.type == SIMPLE_CMD);
	assert (second.type == SIMPLE_CMD);

	pid_t pipe_fd[2];
	pipe(pipe_fd); // bad must check out

	t_io first_io = (t_io){io.input, pipe_fd[WRITE]};
	t_command_result res_first = execute_simple_command(state, first.simple, first_io);

	t_io second_io = (t_io){pipe_fd[READ], io.output};
	t_command_result res_second;

	if (second.type == SIMPLE_CMD)
		res_second = execute_simple_command(state, second.simple, second_io);
	else
		assert ("second must be a simple command" == NULL);


	pidl_push_back_link(pids, res_first.pids);
	pidl_push_back_link(pids, res_second.pids);

	wait_for_all_pids(*pids);

	return (t_command_result){}; // bad dummy
}

t_command_result execute_simple_command(t_state *state, t_simple *simple, t_io io)
{
	t_error err;
	t_pid_list* pids = pidl_new(0);
	if (pids == NULL)
	{
		free(pids);
		return (t_command_result){.error = E_OOM, .must_exit = true, .pids = NULL};
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		free(pids);
		return (t_command_result){.error = E_FORK, .must_exit = true, .pids = NULL};
	}
	if (pid != 0)
	{
		pids->pid = pid;
		return (t_command_result){.error = NO_ERROR, .pids = pids};
	}

	err = perform_all_expansions_on_words(simple->words);
	if (err != NO_ERROR)
		graceful_exit_from_child();

	char *command_path;
	err = path_expanded_word(state->env, simple->words->contents, &command_path);
	if (err != NO_ERROR)
		graceful_exit_from_child(); // bad, should exit with status 127 if the
									// command could not be found and 126 if we don't have
									// execution permissions to the candiate executable

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("dup2");

	int *fds_to_close;
	err = apply_redirections(simple->redirections, &fds_to_close);
	if (err != NO_ERROR)
		graceful_exit_from_child();

	char** argv = wl_into_word_array(&simple->words);
	char** envp = env_make_envp(state->env);
	execve(command_path, argv, envp);

	graceful_exit_from_child();
}
