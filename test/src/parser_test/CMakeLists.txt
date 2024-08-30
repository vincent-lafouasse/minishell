add_executable(parser_test)

set(tokenize_module
  tokenize/tokenize.c
  tokenize/token_repr.c
  tokenize/t_lexer_init.c
  tokenize/lexer_scan_next_token.c
  tokenize/t_token_list/tkl_push_back.c
  tokenize/t_token_list/tkl_new.c
  tokenize/lexer_advance_peek.c
  tokenize/t_token_list/t_token_list.h
  tokenize/t_token.h
  tokenize/t_lexer.h
  tokenize/tokenize.h
)
list(TRANSFORM tokenize_module PREPEND ${tested_src_dir}/)

target_sources(parser_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp

	${tokenize_module}
)

target_include_directories(parser_test PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	${tested_include_dir}
)

target_link_libraries(parser_test
	libgtest
	libft
)
