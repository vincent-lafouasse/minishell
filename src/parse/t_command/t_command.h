/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_command.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:57:58 by poss              #+#    #+#             */
/*   Updated: 2024/11/12 20:11:06 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_COMMAND_H
# define T_COMMAND_H

# include "redirection/t_redir_list/t_redir_list.h"
# include "word/t_word_list/t_word_list.h"
# include <stdbool.h>

typedef enum e_conditional_operator
{
	AND_OP,
	OR_OP,
}	t_conditional_operator;

typedef enum e_command_type
{
	CMD_PIPELINE,
	CMD_CONDITIONAL,
	CMD_SUBSHELL,
	CMD_SIMPLE,
}	t_command_type;

typedef struct s_pipeline		t_pipeline;
typedef struct s_conditional	t_conditional;
typedef struct s_subshell		t_subshell;
typedef struct s_simple			t_simple;

typedef struct s_command
{
	t_command_type		type;
	union
	{
		t_pipeline		*pipeline;
		t_conditional	*conditional;
		t_subshell		*subshell;
		t_simple		*simple;
		void			*any;
	};
}						t_command;

typedef struct s_pipeline
{
	t_command	first;
	t_command	second;
}				t_pipeline;

typedef struct s_conditional
{
	t_conditional_operator	op;
	t_command				first;
	t_command				second;
}							t_conditional;

typedef struct s_subshell
{
	t_command		cmd;
	t_redir_list	*redirections;
}					t_subshell;

typedef struct s_simple
{
	t_word_list		*words;
	t_redir_list	*redirections;
}					t_simple;

t_command		command_new_empty(t_command_type type);

t_pipeline		*pipeline_new(t_command first,
					t_command second);
t_command		command_from_pipeline(t_pipeline *pipeline);
t_command		command_new_pipeline(t_command first,
					t_command second);

t_conditional	*conditional_new(
					t_conditional_operator op,
					t_command first,
					t_command second);
t_command		command_from_conditional(
					t_conditional *conditional);
t_command		command_new_conditional(
					t_conditional_operator op,
					t_command first, t_command second);

t_subshell		*subshell_new(t_command cmd,
					t_redir_list *redirections);
t_command		command_from_subshell(t_subshell *subshell);
t_command		command_new_subshell(t_command cmd,
					t_redir_list *redirections);

t_simple		*simple_new(t_word_list *words,
					t_redir_list *redirections);
t_command		command_from_simple(t_simple *simple);
t_command		command_new_simple(t_word_list *words,
					t_redir_list *redirections);

void			pipeline_destroy(t_pipeline *pipeline);
void			conditional_destroy(t_conditional *conditional);
void			subshell_destroy(t_subshell *subshell);
void			simple_destroy(t_simple *simple);
void			command_destroy_and_clear(t_command *command);
void			command_destroy(t_command command);

bool			command_is_initialized(t_command command);

#endif
