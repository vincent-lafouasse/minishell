add_executable(lexer_test)

target_sources(lexer_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/test.cpp

	${tested_files}
)

target_include_directories(lexer_test PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	${tested_include_dir}
)

target_link_libraries(lexer_test
	libgtest
)
