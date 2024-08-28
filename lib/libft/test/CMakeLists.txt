cmake_minimum_required(VERSION 3.10)

project(libft_tests LANGUAGES C)

include(Unity/CmakeLists.txt)

add_executable(libft_tests)
target_sources(libft_tests PRIVATE
	./src/main.c
	./src/test_atoi.c
	./src/test_bzero.c
	./src/test_calloc.c
	./src/test_ctype.c
	./src/test_memcmp.c
	./src/test_memcpy.c
	./src/test_memset.c
	./src/test_strchr.c
	./src/test_strdup.c
	./src/test_strlcat.c
	./src/test_strlcpy.c
	./src/test_strlen.c
	./src/test_strncmp.c
	./src/utils.c
)

target_include_directories(libft_tests PUBLIC
	./include
	./Unity/include
)


set(libft_dir
  ${CMAKE_SOURCE_DIR}/../
)
add_library(libft STATIC IMPORTED)
set_target_properties(libft PROPERTIES
  IMPORTED_LOCATION "${libft_dir}/libft.a"
  INTERFACE_INCLUDE_DIRECTORIES "${libft_dir}/include"
)

target_link_libraries(libft_tests
	unity
	libft
)

include(CTest)
enable_testing()

add_test(NAME LibftTest
         COMMAND libft_tests)
