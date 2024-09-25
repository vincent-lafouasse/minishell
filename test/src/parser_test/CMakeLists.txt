add_executable(parser_test)

set(token_list_source
	tokenize/token_repr.c
	tokenize/t_token_list/tkl_clear.c
	tokenize/t_token_list/tkl_delone.c
	tokenize/t_token_list/tkl_push_back.c
	tokenize/t_token_list/t_token_list.h
	tokenize/t_token_list/tkl_new.c
	tokenize/t_token.h
)

set(parser_source
	parse/parse.h
	parse/t_parse_tree/productions.h
	parse/t_parse_tree/t_parse_tree.h
	parse/t_parse_tree/parser.c
	parse/parse_command.c
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
	${token_list_source}
	${parser_source}
	${error_module}
	${log_module}
)
list(TRANSFORM tested_source PREPEND ${tested_src_dir}/)

add_library(tested)
target_sources(tested PRIVATE ${tested_source})
target_include_directories(tested PUBLIC ${tested_include_dir})

target_sources(parser_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/parser_test.cpp
)

target_include_directories(parser_test PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	${tested_include_dir}
)

target_link_libraries(parser_test
	libgtest
	libft
	tested
)
