#include <stdio.h>
#include "parse.h"
#include "t_parse_tree/t_parse_tree.h"
#include "t_parse_tree/productions.h"
#include "tokenize/t_token.h"

t_command parse_command(t_token_list *tokens)
{
	t_parser	parser;
	t_parser_error	err;

	parser = parser_new(tokens);
	err = produce_start(&parser);
	if (err.type != NO_PARSE_ERROR)
		printf("encountered parse error near %s\n", token_repr(*(err.token)));
	(void)tokens;
	return (t_command){};
}

t_parser_error	produce_start(t_parser *state)
{
	if (produce_complete_command(state).type == NO_PARSE_ERROR)
	{
		if (parser_matches(state, EOF_TOKEN))
			return parser_ok();
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_complete_command(t_parser *state)
{
	if (produce_pipeline(state).type == NO_PARSE_ERROR)
	{
		return (produce_complete_command_rest(state));
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_complete_command_rest(t_parser *state)
{
	if (parser_matches(state, AND_AND))
	{
		if (produce_pipeline(state).type == NO_PARSE_ERROR)
		{
			return (produce_complete_command_rest(state));
		}
	}
	else if (parser_matches(state, OR_OR))
	{
		if (produce_pipeline(state).type == NO_PARSE_ERROR)
		{
			return (produce_complete_command_rest(state));
		}
	}
	else if (parser_matches(state, L_PAREN) || parser_matches(state, EOF_TOKEN))
		return parser_ok();
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_pipeline(t_parser *state)
{
	if (produce_command(state).type == NO_PARSE_ERROR)
	{
		return (produce_pipeline_rest(state));
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_pipeline_rest(t_parser *state)
{

	if (parser_matches(state, PIPE_TOKEN))
	{
		if (produce_command(state).type == NO_PARSE_ERROR)
		{
			return (produce_pipeline_rest(state));
		}
	}
	else if (parser_matches(state, AND_AND) || parser_matches(state, OR_OR) || \
			 parser_matches(state, R_PAREN) || parser_matches(state, EOF_TOKEN))
		return parser_ok();
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_command(t_parser *state)
{
	if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET) || parser_matches(state, DL_ANGLE_BRACKET) || \
		parser_matches(state, WORD))
	{
		return (produce_simple_command(state));
	}
	else if (parser_matches(state, L_PAREN))
	{
		if (produce_subshell(state).type == NO_PARSE_ERROR)
		{
			return (produce_subshell_precedes(state));
		}
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_subshell_precedes(t_parser *state)
{
	if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET) || parser_matches(state, DL_ANGLE_BRACKET))
	{
		return (produce_redirect_list(state));
	}
	else if (parser_matches(state, AND_AND) || parser_matches(state, OR_OR) || \
		parser_matches(state, PIPE_TOKEN) || parser_matches(state, R_PAREN) || \
		parser_matches(state, EOF_TOKEN))
		return (parser_ok());
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_subshell(t_parser *state)
{
	if (parser_matches(state, L_PAREN))
	{
		if (produce_complete_command(state).type == NO_PARSE_ERROR)
			if (parser_matches(state, R_PAREN))
				return (parser_ok());
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_simple_command(t_parser *state)
{
	if (parser_matches(state, WORD))
	{
		return (produce_maybe_cmd_suffix(state));
	}
	else if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET) || parser_matches(state, DL_ANGLE_BRACKET))
	{
		if (produce_cmd_prefix(state).type == NO_PARSE_ERROR)
		{
			return (produce_cmd_prefix_precedes(state));
		}
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_maybe_cmd_suffix(t_parser *state)
{
	if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET) || parser_matches(state, DL_ANGLE_BRACKET) || \
		parser_matches(state, WORD))
	{
		return (produce_cmd_suffix(state));
	}
	else if (parser_matches(state, AND_AND) || parser_matches(state, OR_OR) || \
		parser_matches(state, PIPE_TOKEN) || parser_matches(state, R_PAREN) || \
		parser_matches(state, EOF))
	{
		return (parser_ok());
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_cmd_prefix_precedes(t_parser *state)
{
	if (parser_matches(state, WORD))
	{
		return (produce_maybe_cmd_suffix(state));
	}
	else if (parser_matches(state, AND_AND) || parser_matches(state, OR_OR) || \
		parser_matches(state, PIPE_TOKEN) || parser_matches(state, R_PAREN) || \
		parser_matches(state, EOF))
	{
		return (parser_ok());
	}
}

t_parser_error	produce_cmd_prefix(t_parser *state)
{
	if (produce_io_redirect(state).type == NO_PARSE_ERROR)
	{
		return (produce_cmd_prefix_rest(state));
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_cmd_prefix_rest(t_parser *state)
{
	if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET) || parser_matches(state, DL_ANGLE_BRACKET))
	{
		if (produce_io_redirect(state).type == NO_PARSE_ERROR)
		{
			return (produce_cmd_prefix_rest(state));
		}
	}
	else if (parser_matches(state, AND_AND) || parser_matches(state, OR_OR) || \
		parser_matches(state, PIPE_TOKEN) || parser_matches(state, R_PAREN) || \
		parser_matches(state, WORD) || parser_matches(state, EOF_TOKEN))
		return (parser_ok());
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_cmd_suffix(t_parser *state)
{
	if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET) || parser_matches(state, DL_ANGLE_BRACKET))
	{
		if (produce_io_redirect(state).type == NO_PARSE_ERROR)
		{
			return (produce_cmd_suffix_rest(state));
		}
	}
	else if (parser_matches(state, WORD))
		return (produce_cmd_suffix_rest(state));
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));

}

t_parser_error	produce_cmd_suffix_rest(t_parser *state)
{
	if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET) || parser_matches(state, DL_ANGLE_BRACKET))
	{
		if (produce_io_redirect(state).type == NO_PARSE_ERROR)
		{
			return (produce_cmd_suffix_rest(state));
		}
	}
	else if (parser_matches(state, WORD))
		return (produce_cmd_suffix_rest(state));
	else if (parser_matches(state, AND_AND) || parser_matches(state, OR_OR) || \
		parser_matches(state, PIPE_TOKEN) || parser_matches(state, R_PAREN) || \
		parser_matches(state, EOF))
		return (parser_ok());
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_redirect_list(t_parser *state)
{
	if (produce_io_redirect(state).type == NO_PARSE_ERROR)
	{
		return (produce_redirect_list_rest(state));
	}
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_redirect_list_rest(t_parser *state)
{
	if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET) || parser_matches(state, DL_ANGLE_BRACKET))
	{
		if (produce_io_redirect(state).type == NO_PARSE_ERROR)
		{
			return (produce_redirect_list_rest(state));
		}
	}
	else if (parser_matches(state, AND_AND) || parser_matches(state, OR_OR) || \
		parser_matches(state, PIPE_TOKEN) || parser_matches(state, R_PAREN) || \
		parser_matches(state, EOF))
		return (parser_ok());
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_io_redirect(t_parser *state)
{
	if (parser_matches(state, DL_ANGLE_BRACKET))
		return (produce_io_here(state));
	if (parser_matches(state, R_ANGLE_BRACKET) || parser_matches(state, L_ANGLE_BRACKET) || \
		parser_matches(state, DR_ANGLE_BRACKET))
		return (produce_io_file(state));
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_io_file(t_parser *state)
{
	if (parser_matches(state, R_ANGLE_BRACKET))
		return (produce_filename(state));
	if (parser_matches(state, L_ANGLE_BRACKET))
		return (produce_filename(state));
	if (parser_matches(state, DR_ANGLE_BRACKET))
		return (produce_filename(state));
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_filename(t_parser *state)
{
	if (parser_matches(state, WORD))
		return (parser_ok());
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_io_here(t_parser *state)
{
	if (parser_matches(state, DL_ANGLE_BRACKET))
		return (produce_here_end(state));
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}

t_parser_error	produce_here_end(t_parser *state)
{
	if (parser_matches(state, WORD))
		return (parser_ok());
	return (parser_error(E_UNEXPECTED_TOKEN, parser_peek_token(state)));
}
