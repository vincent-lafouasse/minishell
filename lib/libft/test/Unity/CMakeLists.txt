cmake_minimum_required(VERSION 3.10)

project(Unity LANGUAGES C)

add_library(unity STATIC)

target_sources(unity
	PRIVATE
		./src/unity.c
		./src/unity_fixture.c
		./src/unity_memory.c
)

target_include_directories(unity
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
