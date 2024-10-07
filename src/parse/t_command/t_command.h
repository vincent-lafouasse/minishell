#pragma once

#include "word/t_word_list/t_word_list.h"
#include "redirection/t_redir_list/t_redir_list.h"
#include <stdbool.h>

typedef enum e_conditional_operator {
	AND_OP,
	OR_OP,
} t_conditional_operator;

typedef enum e_command_type {
	PIPELINE_CMD,
	CONDITIONAL_CMD,
	SUBSHELL_CMD,
	SIMPLE_CMD,
} t_command_type;

typedef struct s_pipeline t_pipeline;
typedef struct s_conditional t_conditional;
typedef struct s_subshell t_subshell;
typedef struct s_simple t_simple;

typedef struct s_command {
	t_command_type type;
	union {
		t_pipeline *pipeline;
		t_conditional *conditional;
		t_subshell *subshell;
		t_simple *simple;
		void *any;
	};
} t_command;

typedef struct s_pipeline {
	t_command first;
	t_command second;
} t_pipeline;

typedef struct s_conditional {
	t_conditional_operator op;
	t_command first;
	t_command second;
} t_conditional;

typedef struct s_subshell {
	t_command cmd;
	t_redir_list *redirections;
} t_subshell;

typedef struct s_simple {
	t_word_list	*words;
	t_redir_list *redirections;
} t_simple;

t_command	command_new_empty(t_command_type type);

t_pipeline	*pipeline_new(t_command first, t_command second);
t_command	command_from_pipeline(t_pipeline *pipeline);
t_command	command_new_pipeline(t_command first, t_command second);

t_conditional	*conditional_new(t_conditional_operator op, t_command first, t_command second);
t_command	command_from_conditional(t_conditional *conditional);
t_command	command_new_conditional(t_conditional_operator op, t_command first, t_command second);

t_subshell	*subshell_new(t_command cmd, t_redir_list *redirections);
t_command	command_from_subshell(t_subshell *subshell);
t_command	command_new_subshell(t_command cmd, t_redir_list *redirections);

t_simple	*simple_new(t_word_list	*words, t_redir_list *redirections);
t_command	command_from_simple(t_simple *simple);
t_command	command_new_simple(t_word_list	*words, t_redir_list *redirections);

bool		command_is_initialized(t_command command);
