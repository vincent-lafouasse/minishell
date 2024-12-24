#include "t_redir_list.h"
#include "error/t_error.h"

#include "libft/string.h"
#include "word/expansions/expand.h"
#include "execute/execute.h" // TODO: move t_state out of this header

#include <fcntl.h>
#include <stdlib.h>
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
	return -1;
}

static int redirectee_fd_for_redir_kind(t_redir_kind kind)
{
	if (kind == FROM_FILE || kind == HERE_DOCUMENT)
		return STDIN_FILENO;
	else if (kind == INTO_FILE || kind == APPEND_INTO_FILE)
		return STDOUT_FILENO;
	return -1;
}

static t_error redirect_expand(t_expansion_variables vars, const char *word, char **out)
{
	t_error err;

	err = variable_expand_word(vars, word, out);
	if (err != NO_ERROR)
		return (err);
	if (*out == NULL)
		return (E_AMBIGUOUS_REDIRECT);
	return (NO_ERROR);
}

static t_error redirect_regular_file(t_expansion_variables vars, t_redirect redir)
{
	int fd;
	int redirectee;
	int errno_backup;
	char *expanded_filename;
	t_error err;

	assert(redir.kind == FROM_FILE || redir.kind == INTO_FILE \
		   || redir.kind == APPEND_INTO_FILE);

	err = redirect_expand(vars, redir.filename, &expanded_filename);
	if (err != NO_ERROR)
		return (err);
	fd = open(expanded_filename, open_flags_for_redir_kind(redir.kind), 0666);
	if (fd < 0)
		return (free(expanded_filename), E_OPEN);
	redirectee = redirectee_fd_for_redir_kind(redir.kind);
	if (fd != redirectee)
	{
		if (dup2(fd, redirectee) < 0)
		{
			errno_backup = errno;
			close(fd);
			errno = errno_backup;
			return (free(expanded_filename), E_DUP2);
		}
		close(fd);
	}
	free(expanded_filename);
	return (NO_ERROR);
}

#define HEREDOC_FILE_PATH "/tmp/minishell-heredoc-XXXXXX"

// some concerns/error cases (ranked by severity):
// - here we error out if the file already exists, this technically enables the
//   user to make minishell error out if they create the file. we do this using
//   the `O_EXCL` flag so we can ensure we are the ones creating the file, this
//   in turns ensures that we have both read and write permissions. simply
//   truncating and not using `O_EXCL` theoretically enables a data race, and is
//   prone to failure if we do not have both write and read permissions on the
//   file that already existed but may be preferable as it is easier to justify
//   during defense.
// - no random filename, as all of the allowed functions are
//   deterministic/filesystem related, and calling `/dev/random` just seems
//   weird (what if you can't open it?)
// - real bash errors out and sets errno to `ENOSPC` if `write` couldn't write
//   the entire document
static t_error redirect_here_document(t_redirect redir)
{
	int tmpfile_writable;
	int tmpfile_read_only;

	assert(redir.kind == HERE_DOCUMENT);

	tmpfile_writable = open(HEREDOC_FILE_PATH, O_RDWR | O_CREAT | O_TRUNC | O_EXCL, S_IRUSR | S_IWUSR);
	if (tmpfile_writable < 0) /* the file is already taken if errno == EEXIST */
		return (E_OPEN);
	tmpfile_read_only = open(HEREDOC_FILE_PATH, O_RDONLY, S_IRUSR);
	if (tmpfile_read_only < 0)
	{
		int err = errno;
		close(tmpfile_writable);
		errno = err;
		return (E_OPEN);
	}
	if (unlink(HEREDOC_FILE_PATH) < 0)
	{
		int err = errno;
		close(tmpfile_writable);
		close(tmpfile_read_only);
		errno = err;
		return (E_UNLINK);
	}
	if (write(tmpfile_writable, redir.doc.contents, ft_strlen(redir.doc.contents)) < 0)
	{
		int err = errno;
		close(tmpfile_writable);
		close(tmpfile_read_only);
		errno = err;
		return (E_WRITE);
	}
	close(tmpfile_writable);
	int redirectee = redirectee_fd_for_redir_kind(redir.kind);
	if (tmpfile_read_only != redirectee)
	{
		if (dup2(tmpfile_read_only, redirectee) < 0)
		{
			int err = errno;
			close(tmpfile_read_only);
			errno = err;
			return (E_DUP2);
		}
		close(tmpfile_read_only);
	}
	return (NO_ERROR);
}

t_error apply_redirections(t_state *state, t_redir_list *redirections)
{
	t_redirect redir;
	t_error err;
	t_expansion_variables vars;

	vars = (t_expansion_variables){.env = state->env, .last_status = state->last_status};
	while (redirections)
	{
		redir = redirections->redirect;

		if (redir.kind == HERE_DOCUMENT)
			err = redirect_here_document(redir);
		else
			err = redirect_regular_file(vars, redir);

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
