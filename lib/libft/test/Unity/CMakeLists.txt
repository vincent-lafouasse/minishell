cmake_minimum_required(VERSION 3.10)

project(Unity LANGUAGES C)

add_library(unity STATIC)

set(unity_root ${CMAKE_CURRENT_LIST_DIR})

target_sources(unity
	PRIVATE
		${unity_root}/src/unity.c
		${unity_root}/src/unity_fixture.c
		${unity_root}/src/unity_memory.c
)

target_include_directories(unity
    PRIVATE
        ${unity_root}/src
        ${unity_root}/include
    PUBLIC
        $<BUILD_INTERFACE:${unity_root}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
