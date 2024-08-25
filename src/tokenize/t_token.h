#pragma once

typedef enum e_token_type
{
	IDENTIFIER,
	PIPE_TOKEN,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	APPEND_TO_FILE,
	HERE_DOC,
	SEPARATOR_TOKEN,
}	t_token_type;

typedef struct s_token {
	t_token_type type;
	char* literal;
} t_token;

const char* token_repr(t_token token);
