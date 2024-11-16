add_executable(expansion_test)

set(word_quotes_list_module
	word/expansions/make_joined.c
	word/expansions/parse.c
	word/expansions/t_word_quotes_list.c
)

set(tested_source
	${word_quotes_list_module}
)
list(TRANSFORM tested_source PREPEND ${tested_src_dir}/)

target_sources(expansion_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/parse.cpp
	${CMAKE_CURRENT_LIST_DIR}/join.cpp

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
