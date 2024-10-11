#include "execute.h"
#include "error/t_error.h"
#include "execute/t_env/t_env.h"
#include "libft/stdlib.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include "word/t_word_list/t_word_list.h"
#include "word/word.h"

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

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
	size_t redir_count;
	size_t i;

	redir_count = rdl_len(redirections);
	/*
	*fds_to_close = ft_calloc(redir_count + 1, sizeof(int));
	if (!*fds_to_close)
		return E_OOM;
	*fds_to_close[redir_count] = -1;
	*/
	i = 0;
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
		i++;
	}
	return (NO_ERROR);
}

t_command_result execute_simple_command(t_state *state, t_simple *simple, t_io io)
{
	pid_t pid;
	char *command_path;
	t_error err;
	char **envp;
	char **argv;
	int *fds_to_close;

	pid = fork();
	if (pid == -1)
		return (t_command_result){.error = E_FORK, .must_exit = true, .exit_status = NEVER_EXITED};
	if (pid != 0)
		return (t_command_result){.error = NO_ERROR, .must_exit = false, .exit_status = NEVER_EXITED};


	err = perform_all_expansions_on_words(simple->words);
	if (err != NO_ERROR)
		return (t_command_result){.error = err, .must_exit = true};
	err = path_expanded_word(state->env, simple->words->contents, &command_path);
	if (err != NO_ERROR)
		return (t_command_result){.error = err, .must_exit = true};

	err = do_piping(io);
	if (err != NO_ERROR)
		perror("dup2");
	err = apply_redirections(simple->redirections, &fds_to_close);
	if (err != NO_ERROR)
		return (t_command_result){.error = err, .must_exit = true}; // bad, should differentiate `apply_redirections` errors

	argv = wl_into_word_array(&simple->words);
	envp = env_make_envp(state->env);
	execve(command_path, argv, envp);

	exit(EXIT_FAILURE); // bad, should clean up all allocations before exiting from child process

	return (t_command_result){.error = E_EXECVE, .must_exit = true, .exit_status = NEVER_EXITED};
}
