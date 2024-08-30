#include "t_lexer.h"
#include "tokenize.h"
#include "log/log.h"
#include <stdio.h>

t_token_list	*tokenize(const char *input)
{
	t_lexer	lexer;
	t_token	token;
	t_error	err;

	lexer = lexer_init(input);
	while (lexer.current < lexer.src_len)
	{
		if (lexer_peek(&lexer) == ' ')
		{
			lexer_advance(&lexer);
			continue;
		}
		lexer.start = lexer.current;
		err = lexer_scan_next_token(&lexer, &token);
		if (err != NO_ERROR)
		{
			log_token_list(lexer.tokens);
			printf("Error:\t%s\n", error_repr(err));
			// cleanup tokens
			return NULL;
		}
		err = tkl_push_back(&lexer.tokens, token);
	}
	tkl_push_back(&lexer.tokens, (t_token){.type = EOF_TOKEN});
	log_token_list(lexer.tokens);
	return (lexer.tokens);
}
