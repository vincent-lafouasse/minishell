add_executable(expansion_test)

set(word_expansions_module
	word/expansions/remove_outer_quotes.c
	word/expansions/expand_variable.c
	word/expansions/t_word_quotes_list.c
	word/expansions/make_joined.c
	word/expansions/parse.c
	word/expansions/pathname/expand.c
	word/expansions/expand.c
)
set(t_string_module
	word/t_string/c_string.c
	word/t_string/insert.c
	word/t_string/alloc.c
	word/t_string/destructor.c
	word/t_string/realloc.c
	word/t_string/constructor.c
)
set(env_module
	execute/t_env/t_env_internals.c
	execute/t_env/getter.c
	execute/t_env/constructor.c
)

set(tested_source
	${word_expansions_module}
	${t_string_module}
	${env_module}
)
list(TRANSFORM tested_source PREPEND ${tested_src_dir}/)

target_sources(expansion_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/parse.cpp
	${CMAKE_CURRENT_LIST_DIR}/join.cpp
	${CMAKE_CURRENT_LIST_DIR}/expand.cpp
	${CMAKE_CURRENT_LIST_DIR}/Variables.cpp

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
