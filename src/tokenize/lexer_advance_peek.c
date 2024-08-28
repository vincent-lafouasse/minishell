#include "t_lexer.h"

char lexer_advance(t_lexer* lexer)
{
	
	return lexer->source[lexer->current++];
}

char lexer_peek(t_lexer* lexer)
{
	return lexer->source[lexer->current];
}
