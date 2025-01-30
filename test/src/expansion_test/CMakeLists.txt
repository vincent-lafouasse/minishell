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
set(env_module
	execute/t_env/t_env.h
	execute/t_env/t_env_internals.h
	execute/t_env/getter.c
	execute/t_env/push.c
	execute/t_env/destructor.c
	execute/t_env/t_env_internals.c
	execute/t_env/t_env.c
	execute/t_env/constructor.c
)
set(word_module
	word/t_string/realloc.h
	word/t_string/t_string.h
	word/t_string/c_string.c
	word/t_string/insert.c
	word/t_string/alloc.c
	word/t_string/destructor.c
	word/t_string/realloc.c
	word/t_string/constructor.c
	word/word_clone.c
	word/expansions/remove_outer_quotes.c
	word/expansions/expand_variable.c
	word/expansions/quote_removed_word.c
	word/expansions/t_word_quotes_list.c
	word/expansions/expand.h
	word/expansions/make_joined.c
	word/expansions/expand_internals.h
	word/expansions/parse.c
	word/expansions/wql_push.c
	word/expansions/pathname/expand_internals.c
	word/expansions/pathname/expand.c
	word/expansions/expand.c
	word/t_word_list/destroy.c
	word/t_word_list/push.c
	word/t_word_list/t_word_list.h
	word/t_word_list/t_word_list.c
	word/word.h
)


set(tested_source
	${word_expansions_module}
	${env_module}
	${word_module}
)
list(TRANSFORM tested_source PREPEND ${tested_src_dir}/)

target_sources(expansion_test PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/parse.cpp
	${CMAKE_CURRENT_LIST_DIR}/expand.cpp
	${CMAKE_CURRENT_LIST_DIR}/empty_expansion.cpp
	${CMAKE_CURRENT_LIST_DIR}/Variables.cpp
	${CMAKE_CURRENT_LIST_DIR}/Words.cpp

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
