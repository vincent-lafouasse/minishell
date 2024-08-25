#pragma once

typedef enum e_token
{
	IDENTIFIER,
	PIPE,
	LEFT_ANGLE_BRACKET,
	LEFT_LEFT_ANGLE_BRACKET,
	RIGHT_ANGLE_BRACKET,
	RIGHT_RIGHT_ANGLE_BRACKET,
	SEMI_COLON,
}	t_token;

struct TokenList; // not sure what

struct TokenList tokenize(const char* input);
