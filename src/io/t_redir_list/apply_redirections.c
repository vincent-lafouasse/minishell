#include "t_redir_list.h"

#include <fcntl.h>
#include <unistd.h>

static int open_flags_for_redir_kind(t_redir_kind kind)
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

t_error apply_redirections(t_redir_list *redirections)
{
	t_redirect redir;
	int fd;

	while (redirections)
	{
		redir = redirections->redirect;

		if (redir.kind == HERE_DOCUMENT)
			return (E_DUMMY);

		/*
		err = expand(redir.filename, &expanded_filename); // TODO: check for ambiguous redirections, i.e. when expansion expands to either zero or more than one word
		*/

		fd = open(redir.filename, O_CLOEXEC | open_flags_for_redir_kind(redir.kind), 0666); // XXX: why tf are we O_CLOEXECing
		if (fd == -1)
			return E_DUMMY; // bad, should short circuit command evaluation if file does not exist
		if (redir.kind == FROM_FILE || redir.kind == HERE_DOCUMENT)
			dup2(fd, STDIN_FILENO); // bad, should report dup2 error
		else if (redir.kind == INTO_FILE || redir.kind == APPEND_INTO_FILE)
			dup2(fd, STDOUT_FILENO); // bad, should report dup2 error

		redirections = redirections->next;
	}
	return (NO_ERROR);
}
