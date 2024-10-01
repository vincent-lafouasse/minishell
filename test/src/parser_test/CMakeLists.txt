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
	parse/t_symbol.c
	parse/t_parser.h
	parse/productions/produce_complete_command_rest.c
	parse/productions/produce_program.c
	parse/productions/produce_word_list_rest.c
	parse/productions/produce_command.c
	parse/productions/produce_complete_command.c
	parse/productions/produce_pipeline_rest.c
	parse/productions/productions.h
	parse/productions/produce_pipeline.c
	parse/productions/produce_word_list.c
	parse/t_command/t_command.h
	parse/parse.c
	parse/t_symbol.h

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

target_sources(parser_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/parser_test.cpp

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
