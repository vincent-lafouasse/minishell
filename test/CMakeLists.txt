cmake_minimum_required(VERSION 3.10)

project(minishell)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # LSP stuff, don't worry about it

# fetch gtest
# targets: `libgtest` `libgmock` (static libraries)
include(${CMAKE_SOURCE_DIR}/fetch_gtest.cmake)

set(libft_dir
  ${CMAKE_SOURCE_DIR}/../lib/libft
)
add_library(libft STATIC IMPORTED)
set_target_properties(libft PROPERTIES
  IMPORTED_LOCATION "${libft_dir}/libft.a"
  INTERFACE_INCLUDE_DIRECTORIES "${libft_dir}/include"
)

set(tested_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
set(tested_include_dir ${tested_src_dir}/)

set(tested_files
)
list(TRANSFORM tested_files PREPEND ${tested_src_dir}/)

set(tokenize_module
  src/tokenize/tokenize.c
  src/tokenize/token_repr.c
  src/tokenize/t_lexer_init.c
  src/tokenize/lexer_scan_next_token.c
  src/tokenize/t_token_list/tkl_push_back.c
  src/tokenize/t_token_list/tkl_new.c
  src/tokenize/lexer_advance_peek.c
  src/tokenize/t_token_list/t_token_list.h
  src/tokenize/t_token.h
  src/tokenize/t_lexer.h
  src/tokenize/tokenize.h
)

include(./src/lexer_test/CMakeLists.txt)
include(./src/parser_test/CMakeLists.txt)

include(CTest)
enable_testing()

add_test(NAME LexerTest
         COMMAND lexer_test)
add_test(NAME ParserTest
         COMMAND parser_test)
