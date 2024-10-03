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
	parse/t_command/properties.c
	parse/t_command/t_command.h
	parse/t_command/empty.c
	parse/t_command/constructor.c
	parse/parse.c
	parse/t_parser/t_parser.c
	parse/t_parser/t_parser.h
	parse/t_symbol/t_symbol.c
	parse/t_symbol/t_symbol.h
	parse/t_symbol/t_symbol_stack/t_symbol_stack.h
	parse/t_symbol/t_symbol_stack/t_symbol_stack.c
)

set(error_module
	error/t_error.h
	error/error_repr.c
)
set(log_module
	#log/log_words.c
	log/log_token.c
	log/log_error.c
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
