/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:54:02 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 20:05:25 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/t_error.h"
#include "libft/string.h"
#include "shell/shell.h"
#include "t_redir_list.h"
#include "word/expansions/expand.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// closes old_fd in case of error
static t_error	move_fd_if_not_same(int old_fd, int new_fd)
{
	int	errno_backup;

	if (old_fd != new_fd)
	{
		if (dup2(old_fd, new_fd) < 0)
		{
			errno_backup = errno;
			close(old_fd);
			errno = errno_backup;
			return (E_DUP2);
		}
		close(old_fd);
	}
	return (NO_ERROR);
}

static t_error	redirect_regular_file(t_expansion_variables vars,
		t_redirect redir)
{
	int		fd;
	int		redirectee;
	char	*expanded_filename;
	t_error	err;

	assert(redir.kind == FROM_FILE || redir.kind == INTO_FILE
		|| redir.kind == APPEND_INTO_FILE);
	err = variable_expand_word(vars, redir.filename, &expanded_filename);
	if (err != NO_ERROR)
		return (err);
	if (expanded_filename == NULL)
		return (E_AMBIGUOUS_REDIRECT);
	fd = open(expanded_filename, open_flags_for_redir_kind(redir.kind), 0666);
	if (fd < 0)
		return (free(expanded_filename), E_OPEN);
	redirectee = redirectee_fd_for_redir_kind(redir.kind);
	err = move_fd_if_not_same(fd, redirectee);
	if (err != NO_ERROR)
		return (free(expanded_filename), E_DUP2);
	free(expanded_filename);
	return (NO_ERROR);
}

#define HEREDOC_FILE_PATH "/tmp/minishell-heredoc-XXXXXX"

// if (*wr_out < 0) /* the file is already taken if errno == EEXIST */
static t_error	make_tmp_file(const char *path, int *wr_out, int *rd_out)
{
	int	errno_backup;

	*wr_out = open(path, O_RDWR | O_CREAT | O_TRUNC | O_EXCL,
			S_IRUSR | S_IWUSR);
	if (*wr_out < 0)
		return (E_OPEN);
	*rd_out = open(path, O_RDONLY, S_IRUSR);
	if (*rd_out < 0)
	{
		errno_backup = errno;
		close(*wr_out);
		errno = errno_backup;
		return (E_OPEN);
	}
	if (unlink(HEREDOC_FILE_PATH) < 0)
	{
		errno_backup = errno;
		close(*wr_out);
		close(*rd_out);
		errno = errno_backup;
		return (E_UNLINK);
	}
	return (NO_ERROR);
}

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
static t_error	redirect_here_document(t_redirect redir)
{
	int		tmpfile_writable;
	int		tmpfile_rd_only;
	t_error	err;
	int		errno_store;
	int		redirectee;

	assert(redir.kind == HERE_DOCUMENT);
	err = make_tmp_file(HEREDOC_FILE_PATH, &tmpfile_writable, &tmpfile_rd_only);
	if (err != NO_ERROR)
		return (err);
	if (write(tmpfile_writable, redir.doc.contents,
			ft_strlen(redir.doc.contents)) < 0)
	{
		errno_store = errno;
		close(tmpfile_writable);
		close(tmpfile_rd_only);
		errno = errno_store;
		return (E_WRITE);
	}
	close(tmpfile_writable);
	redirectee = redirectee_fd_for_redir_kind(redir.kind);
	err = move_fd_if_not_same(tmpfile_rd_only, redirectee);
	if (err != NO_ERROR)
		return (err);
	return (NO_ERROR);
}

// we would undo the redirections, but the only case where that matters
// is when running a builtin,since there's no fork. we already take care of
// that case by saving stdin and stdout (we don't care to redirect other
// streams than 0 and 1). otherwise, since we're in a child process, we
// will exit right after. although this now means that there are open
// file descriptors when the process exits...
t_error	apply_redirections(t_state *state, t_redir_list *redirections)
{
	t_redirect				redir;
	t_error					err;
	t_expansion_variables	vars;

	vars = (t_expansion_variables){.env = state->env,
		.last_status = state->last_status};
	while (redirections)
	{
		redir = redirections->redirect;
		if (redir.kind == HERE_DOCUMENT)
			err = redirect_here_document(redir);
		else
			err = redirect_regular_file(vars, redir);
		if (err != NO_ERROR)
			return (err);
		redirections = redirections->next;
	}
	return (NO_ERROR);
}
