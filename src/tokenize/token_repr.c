#include "t_token.h"

#define DUMMY_TOKEN_REPR "that's a token for sure"
#define IDENTIFIER_REPR "Identifier"
#define WORD_REPR "Word"
#define PIPE_TOKEN_REPR "Pipe"
#define L_ANGLE_BRACKET_REPR "<"
#define DL_ANGLE_BRACKET_REPR "<<"
#define R_ANGLE_BRACKET_REPR ">"
#define DR_ANGLE_BRACKET_REPR ">>"
#define L_PAREN_REPR "("
#define R_PAREN_REPR ")"
#define AND_AND_REPR "&&"
#define OR_OR_REPR "||"
#define EOF_TOKEN_REPR "EOF"
#define UNRECOGNIZED_TOKEN "Unrecognized token"

const char	*token_repr(t_token token)
{
	if (token.type == IDENTIFIER)
		return IDENTIFIER_REPR;
	if (token.type == WORD)
		return WORD_REPR;
	if (token.type == PIPE_TOKEN)
		return PIPE_TOKEN_REPR;
	if (token.type == L_ANGLE_BRACKET)
		return L_ANGLE_BRACKET_REPR;
	if (token.type == DL_ANGLE_BRACKET)
		return DL_ANGLE_BRACKET_REPR;
	if (token.type == R_ANGLE_BRACKET)
		return R_ANGLE_BRACKET_REPR;
	if (token.type == DR_ANGLE_BRACKET)
		return DR_ANGLE_BRACKET_REPR;
	if (token.type == L_PAREN)
		return L_PAREN_REPR;
	if (token.type == R_PAREN)
		return R_PAREN_REPR;
	if (token.type == AND_AND)
		return AND_AND_REPR;
	if (token.type == OR_OR)
		return OR_OR_REPR;
	if (token.type == EOF_TOKEN)
		return EOF_TOKEN_REPR;
	return (UNRECOGNIZED_TOKEN);
}
