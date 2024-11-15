add_executable(expansion_test)

set(word_quotes_list_module
	word/expansions/t_word_quotes_list/parse.c
	word/expansions/t_word_quotes_list/t_word_quotes_list.c
)

set(tested_source
	${word_quotes_list_module}
)
list(TRANSFORM tested_source PREPEND ${tested_src_dir}/)

target_sources(expansion_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/word_quotes_list.cpp

	${tested_source}
)

target_include_directories(expansion_test PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
	${tested_include_dir}
)

target_link_libraries(expansion_test
	libgtest
	libft
)
