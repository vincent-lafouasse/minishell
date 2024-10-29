add_executable(parser_test)

set(parser_source
	parse/parse.h
	parse/reduction/reduce_simple_command.c
	parse/reduction/reduce_simple_command_like.c
	parse/reduction/gather_leaves.c
	parse/reduction/reduce_complete_command.c
	parse/reduction/reduce_pipeline.c
	parse/reduction/reduction.c
	parse/reduction/reduce_command.c
	parse/reduction/reduce_subshell.c
	parse/reduction/invert_conditional_associativity.c
	parse/reduction/reduction_internals.h
	parse/reduction/reduction.h
	parse/productions/produce_simple_command.c
	parse/productions/produce_io_redirect.c
	parse/productions/produce_redirect_list_rest.c
	parse/productions/produce_complete_command_rest.c
	parse/productions/produce_io_file.c
	parse/productions/produce_program.c
	parse/productions/produce_command.c
	parse/productions/produce_cmd_suffix_rest.c
	parse/productions/produce_subshell.c
	parse/productions/produce_subshell_precedes.c
	parse/productions/produce_redirect_list.c
	parse/productions/produce_complete_command.c
	parse/productions/produce_pipeline_rest.c
	parse/productions/produce_cmd_prefix_precedes.c
	parse/productions/produce_cmd_prefix_rest.c
	parse/productions/produce_cmd_suffix.c
	parse/productions/productions.h
	parse/productions/produce_cmd_prefix.c
	parse/productions/produce_here_end.c
	parse/productions/produce_pipeline.c
	parse/productions/produce_filename.c
	parse/productions/produce_io_here.c
	parse/productions/produce_maybe_cmd_suffix.c
	parse/t_command/destructor.c
	parse/t_command/properties.c
	parse/t_command/t_command.h
	parse/t_command/empty.c
	parse/t_command/constructor.c
	parse/tokenize/tokenize.h
	parse/tokenize/token_repr.c
	parse/tokenize/t_lexer/lexer_scan_metachar_operator.c
	parse/tokenize/t_lexer/lexer_internals.h
	parse/tokenize/t_lexer/lexer_skip_whitespace.c
	parse/tokenize/t_lexer/fill_token.c
	parse/tokenize/t_lexer/t_lexer_init.c
	parse/tokenize/t_lexer/lexer_scan_next_token.c
	parse/tokenize/t_lexer/t_lexer.h
	parse/tokenize/t_lexer/lexer_scan_word.c
	parse/tokenize/t_lexer/lexer_advance_peek.c
	parse/tokenize/t_token_list/tkl_clear.c
	parse/tokenize/t_token_list/tkl_delone.c
	parse/tokenize/t_token_list/tkl_push_back.c
	parse/tokenize/t_token_list/t_token_list.h
	parse/tokenize/t_token_list/tkl_new.c
	parse/tokenize/t_token.h
	parse/tokenize/t_token.c
	parse/tokenize/tokenize.c
	parse/parse.c
	parse/t_parser/t_parser.c
	parse/t_parser/t_parser.h
	parse/t_symbol/t_symbol.c
	parse/t_symbol/t_symbol.h
)

set(word_source
	word/t_word_list/t_word_list.h
	word/t_word_list/t_word_list.c
)

set(io_source
	io/t_redir_list/t_redir_list.h
	io/t_redir_list/t_redir_list.c
	io/t_redir_list/redirection_repr.c
)

set(error_module
	error/t_error.h
	error/error_repr.c
)
set(log_module
	log/log_words.c
	log/syntax_tree_to_json.c
	log/log_token.c
	log/log_error.c
	log/log.h
	log/log_env.c
	log/parse_tree_to_json.c
)

set(tested_source
	${token_list_source}
	${parser_source}
	${word_source}
	${io_source}
	${error_module}
	${log_module}
)
list(TRANSFORM tested_source PREPEND ${tested_src_dir}/)

target_sources(parser_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/parse_tree_test.cpp
	${CMAKE_CURRENT_LIST_DIR}/parser_test.cpp

	${CMAKE_CURRENT_LIST_DIR}/Redirections.cpp
	${CMAKE_CURRENT_LIST_DIR}/Redirections.h
	${CMAKE_CURRENT_LIST_DIR}/Words.cpp
	${CMAKE_CURRENT_LIST_DIR}/Words.h

	${tested_source}
)

target_include_directories(parser_test PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	${tested_include_dir}
)

target_link_libraries(parser_test
	libgtest
	libft
)
