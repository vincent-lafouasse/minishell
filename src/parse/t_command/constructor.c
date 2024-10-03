#include "t_command.h"
#include "libft/stdlib.h"
#include "word/t_word_list/t_word_list.h"
#include "redirection/t_redir_list/t_redir_list.h"

#include <stddef.h>

t_pipeline	*pipeline_new(t_command first, t_command second)
{
	t_pipeline	*pipeline;

	pipeline = ft_calloc(1, sizeof(*pipeline));
	if (!pipeline)
		return NULL;
	*pipeline = (t_pipeline){
		.first = first,
		.second = second,
	};
	return pipeline;
}

t_command	command_from_pipeline(t_pipeline *pipeline)
{
	return (t_command){.type = PIPELINE_CMD, .pipeline = pipeline};
}

t_command	command_new_pipeline(t_command first, t_command second)
{
	//return (t_command){.type = PIPELINE_CMD, .pipeline = pipeline};
}

t_conditional	*conditional_new(t_conditional_operator operator, t_command first, t_command second);
t_command	command_from_conditional(t_conditional *conditional);
t_command	command_new_conditional(t_conditional_operator operator, t_command first, t_command second);

t_subshell	*subshell_new(t_command cmd, t_redir_list *redirections);
t_command	command_from_subshell(t_subshell *subshell);
t_command	command_new_subshell(t_command cmd, t_redir_list *redirections);

t_simple	*simple_new(t_word_list	*words, t_redir_list *redirections);
t_command	command_from_simple(t_simple *simple);
t_command	command_new_simple(t_word_list	*words, t_redir_list *redirections);
