#include "t_redir_list.h"
#include "parse/t_command/t_command.h"
#include "word/expansions/expand.h"
#include "signal/signal.h"

#include "libft/string.h"
#include "libft/ft_string.h"
#include <stdio.h> // HACK: supress readline compilation errors on my end
#include "readline/readline.h"

#include <stdlib.h>
#include <unistd.h>

#define HERE_DOCUMENT_PROMPT "> "

static void warn_for_unexpected_eof(void)
{
	const char *message;

	message = "minishell: warning: here-document delimited by end-of-file\n";
	write(STDERR_FILENO, message, ft_strlen(message));
}

static char *join_delimited(const char *s1, char delim, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*out;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	out = malloc(len1 + 1 + len2 + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, s1, len1);
	out[len1] = delim;
	ft_memcpy(out + len1 + 1, s2, len2);
	out[len1 + 1 + len2] = '\0';
	return (out);
}

static t_error	string_append(char **output, const char *str)
{
	char	*result;

	result = join_delimited(*output, '\n', str);
	if (!result)
		return (E_OOM);
	free(*output);
	*output = result;
	return (NO_ERROR);
}

static bool delimiter_matches_line(const char *delimiter, const char *line)
{
	size_t delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	return (ft_strncmp(line, delimiter, delimiter_len + 1) == 0);
}

// heredoc: bad! the here document should have a trailing newline. this is due
// to the fact that bash makes readline add a trailing newline to every line
// read, but we aren't able to do the same. possible solutions include:
//   - wrap readline and simply add a newline at the end of the string -> more error handling
//   - modify `string_append` to make a string big enough to fit a newline in the middle, and at the end -> more code
static t_error	read_here_document_internal(const char *delimiter, char **document_out)
{
	char *line;

	last_signal = 0; // TODO: ensure that SIGINTs before this point were addressed
	while (1)
	{
		line = readline(HERE_DOCUMENT_PROMPT);
		if (last_signal == SIGINT)
		{
			free(line);
			return (E_INTERRUPTED);
		}
		if (!line)
		{
			warn_for_unexpected_eof();
			break;
		}
		if (delimiter_matches_line(delimiter, line))
		{
			free(line);
			break;
		}
		if (string_append(document_out, line) != NO_ERROR)
		{
			free(line);
			return (E_OOM);
		}
		free(line);
	}
	return (NO_ERROR);
}

static t_error	read_here_document(const char *raw_delimiter, char **document_out)
{
	char *delimiter;
	t_error err;

	delimiter = quote_removed_word(raw_delimiter);
	if (!delimiter)
		return (E_OOM);
	*document_out = ft_strdup("");
	if (*document_out == NULL)
		return (free(delimiter), E_OOM);
	err = read_here_document_internal(delimiter, document_out);
	if (err != NO_ERROR)
	{
		free(*document_out);
		*document_out = NULL;
	}
	free(delimiter);
	return (err);
}

static t_error	read_here_documents_in_rdl(t_redir_list *rdl)
{
	t_here_doc *current_doc;
	t_error err;
	char *document;

	while (rdl)
	{
		if (rdl->redirect.kind != HERE_DOCUMENT)
		{
			rdl = rdl->next;
			continue;
		}
		current_doc = &rdl->redirect.doc;
		err = read_here_document(current_doc->here_doc_eof, &document);
		if (err != NO_ERROR)
			return err;
		current_doc->contents = document;
		printf("here document (%s) contents: %s\n", current_doc->here_doc_eof, document);
		rdl = rdl->next;
	}
	return (NO_ERROR);
}

t_error	gather_here_documents(t_command cmd)
{
	t_error err;

	if (cmd.type == CMD_SIMPLE)
		return read_here_documents_in_rdl(cmd.simple->redirections);
	else if (cmd.type == CMD_SUBSHELL)
	{
		err = gather_here_documents(cmd.subshell->cmd);
		if (err != NO_ERROR)
			return err;
		return read_here_documents_in_rdl(cmd.subshell->redirections);
	}
	else if (cmd.type == CMD_PIPELINE)
	{
		err = gather_here_documents(cmd.pipeline->first);
		if (err != NO_ERROR)
			return err;
		return gather_here_documents(cmd.pipeline->second);
	}
	else if (cmd.type == CMD_CONDITIONAL)
	{
		err = gather_here_documents(cmd.conditional->first);
		if (err != NO_ERROR)
			return err;
		return gather_here_documents(cmd.conditional->second);
	}
	return (NO_ERROR);
}
