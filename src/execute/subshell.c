#include "execute.h"
#include "libft/string.h"
#include "parse/t_command/t_command.h"
#include <unistd.h>
#include <assert.h>

static void warn_non_empty_redirs(const t_subshell* s) {
	const char* msg = "minishell: warning: ignored redirs in subshell\n";
	if (s->redirections != NULL)
		write(STDERR_FILENO, msg, ft_strlen(msg));
}

static t_launch_result launch_subshell(t_state* state, t_subshell* subshell, t_io io);

t_command_result execute_subshell(t_state *state, t_subshell *subshell)
{
	warn_non_empty_redirs(subshell);
	t_command inner = subshell->cmd;
	assert (inner.type == SIMPLE_CMD);

	//t_launch_result launch_result = launch_subshell(state, subshell);
}
