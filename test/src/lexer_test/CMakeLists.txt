add_executable(lexer_test)

set(tokenize_module
	parse/tokenize/tokenize.h
	parse/tokenize/token_repr.c
	parse/tokenize/t_lexer/lexer_scan_metachar_operator.c
	parse/tokenize/t_lexer/lexer_skip_whitespace.c
	parse/tokenize/t_lexer/lexer_internals.h
	parse/tokenize/t_lexer/fill_token.c
	parse/tokenize/t_lexer/t_lexer_init.c
	parse/tokenize/t_lexer/lexer_scan_next_token.c
	parse/tokenize/t_lexer/lexer_scan_string.c
	parse/tokenize/t_lexer/t_lexer.h
	parse/tokenize/t_lexer/lexer_scan_word.c
	parse/tokenize/t_lexer/lexer_advance_peek.c
	parse/tokenize/t_token_list/tkl_clear.c
	parse/tokenize/t_token_list/tkl_delone.c
	parse/tokenize/t_token_list/tkl_push_back.c
	parse/tokenize/t_token_list/t_token_list.h
	parse/tokenize/t_token_list/tkl_new.c
	parse/tokenize/t_token.h
	parse/tokenize/tokenize.c
)
set(error_module
	error/t_error.h
	error/error_repr.c
)
set(log_module
	log/log_error.c
	log/log_token.c
	log/log.h
)

set(tested_source
	${tokenize_module}
	${error_module}
	${log_module}
)
list(TRANSFORM tested_source PREPEND ${tested_src_dir}/)

target_sources(lexer_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/token_list.cpp
	${CMAKE_CURRENT_LIST_DIR}/tokenize.cpp

	${tested_source}
)

target_include_directories(lexer_test PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	${tested_include_dir}
)

target_link_libraries(lexer_test
	libgtest
	libft
)
