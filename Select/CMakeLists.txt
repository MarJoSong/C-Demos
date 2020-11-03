cmake_minimum_required(VERSION 3.16)
project(select C)

set(CMAKE_C_STANDARD 99)

add_executable(server server.c)
add_executable(client client.c)
add_executable(simpleclient simpleclient.c)

#include_directories("${PROJECT_BINARY_DIR}")

#file(GLOB_RECURSE mains RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

#foreach(srcfile IN LISTS mains)
#    get_filename_component(mainname ${srcfile} NAME_WE)
#    add_executable(${mainname} ${srcfile})
#endforeach()