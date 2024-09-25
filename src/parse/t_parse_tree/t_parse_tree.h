#ifndef T_PARSE_TREE_H
# define T_PARSE_TREE_H

# include <stddef.h>
# include "tokenize/t_token.h"
# include "tokenize/t_token_list/t_token_list.h"

typedef enum e_parser_error_type t_parser_error_type;
enum e_parser_error_type {
	NO_PARSE_ERROR,
	E_UNEXPECTED_TOKEN
};

typedef struct s_parser_error t_parser_error;
struct s_parser_error {
	t_parser_error_type	type;
	const t_token		*token;
	//t_token				*near;
};

static t_parser_error parser_ok(void)
{
	return (t_parser_error){NO_PARSE_ERROR, NULL};
}

static t_parser_error parser_error(t_parser_error_type type, const t_token *actual)
{
	return (t_parser_error){type, actual};
}

typedef struct s_parser t_parser;
struct s_parser {
	t_token_list	*_head;
	t_token_list	*current_token;
};

static t_parser	parser_new(t_token_list *tokens)
{
	return ((t_parser){tokens, tokens});
}

bool			parser_matches(t_parser *parser, t_token_type ty);
const t_token	*parser_peek_token(const t_parser *parser);
t_token			*parser_advance_token(t_parser *parser);

#endif // T_PARSE_TREE_H
