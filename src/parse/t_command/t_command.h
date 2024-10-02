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
	t_conditional_operator operator;
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
