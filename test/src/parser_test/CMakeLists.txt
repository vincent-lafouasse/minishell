add_executable(parser_test)

target_sources(parser_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp

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
