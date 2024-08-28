#include "t_lexer.h"
#include "tokenize.h"

t_token_list	*tokenize(const char *input)
{
	t_lexer	lexer;
	t_token	token;
	t_error	err;

	lexer = lexer_init(input);
	while (lexer.current < lexer.src_len)
	{
		lexer.start = lexer.current;
		err = lexer_scan_next_token(&lexer, &token);
		err = tkl_push_back(&lexer.tokens, token);
		(void)err;
	}
	tkl_push_back(&lexer.tokens, (t_token){.type = EOF_TOKEN});
	return (lexer.tokens);
}
