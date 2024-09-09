add_executable(lexer_test)

set(tokenize_module
	tokenize/tokenize.h
	tokenize/token_repr.c
	tokenize/t_lexer/lexer_scan_metachar_operator.c
	tokenize/t_lexer/lexer_skip_whitespace.c
	tokenize/t_lexer/lexer_internals.h
	tokenize/t_lexer/fill_token.c
	tokenize/t_lexer/t_lexer_init.c
	tokenize/t_lexer/lexer_scan_next_token.c
	tokenize/t_lexer/lexer_scan_string.c
	tokenize/t_lexer/t_lexer.h
	tokenize/t_lexer/lexer_scan_word.c
	tokenize/t_lexer/lexer_advance_peek.c
	tokenize/t_token_list/tkl_clear.c
	tokenize/t_token_list/tkl_delone.c
	tokenize/t_token_list/tkl_push_back.c
	tokenize/t_token_list/t_token_list.h
	tokenize/t_token_list/tkl_new.c
	tokenize/t_token.h
	tokenize/tokenize.c
)
set(error_module
	error/t_error.h
	error/error_repr.c
)
set(log_module
	log/log.c
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
