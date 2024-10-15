#include "t_command.h"
#include "io/t_redir_list/t_redir_list.h"
#include "word/t_word_list/t_word_list.h"
#include <stdlib.h>

void	pipeline_destroy(t_pipeline *pipeline)
{
	if (pipeline == NULL)
		return ;
	command_destroy(pipeline->first);
	command_destroy(pipeline->second);
	free(pipeline);
}

void	conditional_destroy(t_conditional *conditional)
{
	if (conditional == NULL)
		return ;
	command_destroy(conditional->first);
	command_destroy(conditional->second);
	free(conditional);
}

void	subshell_destroy(t_subshell *subshell)
{
	if (subshell == NULL)
		return ;
	command_destroy(subshell->cmd);
	rdl_clear(&subshell->redirections, free);
	free(subshell);
}

void	simple_destroy(t_simple *simple)
{
	if (simple == NULL)
		return ;
	rdl_clear(&simple->redirections, free);
	wl_clear(&simple->words, free);
	free(simple);
}

void	command_destroy(t_command command)
{
	if (!command_is_initialized(command))
		return ;
	if (command.type == SIMPLE_CMD)
		return simple_destroy(command.simple);
	else if (command.type == SUBSHELL_CMD)
		return subshell_destroy(command.subshell);
	else if (command.type == PIPELINE_CMD)
		return pipeline_destroy(command.pipeline);
	else if (command.type == CONDITIONAL_CMD)
		return conditional_destroy(command.conditional);
}
