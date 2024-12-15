#include "t_redir_list.h"
#include "error/t_error.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

static int open_flags_for_redir_kind(t_redir_kind kind)
{
	if (kind == FROM_FILE)
		return O_RDONLY;
	else if (kind == INTO_FILE)
		return O_CREAT | O_WRONLY | O_TRUNC;
	else if (kind == APPEND_INTO_FILE)
		return O_CREAT | O_WRONLY | O_APPEND;
	else
		return -1;
}

static int redirectee_fd_for_redir_kind(t_redir_kind kind)
{
	if (kind == FROM_FILE || kind == HERE_DOCUMENT)
		return STDIN_FILENO;
	else if (kind == INTO_FILE || kind == APPEND_INTO_FILE)
		return STDOUT_FILENO;
	else
		return -1;
}

static t_error redirect_regular_file(t_redirect redir)
{
	int fd;
	int redirectee;
	int errno_backup;

	assert(redir.kind == FROM_FILE || redir.kind == INTO_FILE \
		   || redir.kind == APPEND_INTO_FILE);

	/*
	err = expand(redir.filename, &expanded_filename); // TODO: check for ambiguous redirections, i.e. when expansion expands to either zero or more than one word
	*/

	fd = open(redir.filename, open_flags_for_redir_kind(redir.kind), 0666);
	if (fd < 0)
		return (/* free(expanded_filename), */ E_OPEN);
	redirectee = redirectee_fd_for_redir_kind(redir.kind);
	if (fd != redirectee)
	{
		if (dup2(fd, redirectee) < 0)
		{
			errno_backup = errno;
			close(fd);
			errno = errno_backup;
			return (/* free(expanded_filename), */ E_DUP2);
		}
		close(fd);
	}
	// free(expanded_filename);
	return (NO_ERROR);
}

static t_error redirect_here_document(t_redirect redir);

t_error apply_redirections(t_redir_list *redirections)
{
	t_redirect redir;
	t_error err;

	while (redirections)
	{
		redir = redirections->redirect;

		if (redir.kind == HERE_DOCUMENT)
			err = redirect_here_document(redir);
		else
			err = redirect_regular_file(redir);

		// we would undo the redirections, but the only case where that matters
		// is when running a builtin, since there's no fork. we already take care of
		// that case by saving stdin and stdout (we don't care to redirect other
		// streams than 0 and 1). otherwise, since we're in a child process, we
		// will exit right after. although this now means that there are open
		// file descriptors when the process exits...
		if (err != NO_ERROR)
			return (err);

		redirections = redirections->next;
	}
	return (NO_ERROR);
}
