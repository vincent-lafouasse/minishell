#pragma once

typedef enum e_token_type
{
	IDENTIFIER,
	PIPE_TOKEN,
	LEFT_ANGLE_BRACKET,
	LEFT_LEFT_ANGLE_BRACKET,
	RIGHT_ANGLE_BRACKET,
	RIGHT_RIGHT_ANGLE_BRACKET,
	SEMI_COLON_TOKEN,
}	t_token_type;

struct s_token {
	t_token_type type;
	char* literal;
};

typedef struct TokenList {} TokenList; // not sure what

TokenList tokenize(const char* input);
