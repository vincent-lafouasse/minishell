#include "t_redir_list.h"
#include "parse/t_command/t_command.h"
#include "word/expansions/expand.h"
#include "word/t_string/t_string.h"
#include "signal/signal.h"

#include "libft/string.h"
#include "libft/ft_io.h"
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
	ft_putendl_fd(message, STDERR_FILENO);
}

static t_error	here_doc_append(t_string **out, const char *str)
{
	if (string_extend(out, str))
		return (E_OOM);
	if (string_push(out, '\n'))
		return (E_OOM);
	return (NO_ERROR);
}

static bool delimiter_matches_line(const char *delimiter, const char *line)
{
	size_t delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	return (ft_strncmp(line, delimiter, delimiter_len + 1) == 0);
}

static t_error	read_here_document_internal(const char *delimiter, t_string **document_out)
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
		if (here_doc_append(document_out, line) != NO_ERROR)
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
	t_string *str;

	err = quote_removed_word(raw_delimiter, &delimiter);
	if (err != NO_ERROR)
		return (err);
	str = string_new();
	if (!str)
		return (free(delimiter), E_OOM);
	err = read_here_document_internal(delimiter, &str);
	if (err != NO_ERROR)
	{
		free(delimiter);
		string_destroy(str);
		return (err);
	}
	err = string_make_c_string(str, document_out);
	free(delimiter);
	string_destroy(str);
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
		return gather_here_documents(cmd.subshell->cmd); // redirections ignored!
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
