cmake_minimum_required(VERSION 3.16)
project(Socket C)

set(CMAKE_C_STANDARD 11)

add_executable(simpleListen simpleListen.c)

add_executable(server server.c)
target_link_libraries(server -lpthread)