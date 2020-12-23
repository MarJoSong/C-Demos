cmake_minimum_required(VERSION 3.16)
project(Socket C)

set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(hexdump)

add_executable(simpleListen simpleListen.c)
target_link_libraries(simpleListen -lhexdump)

add_executable(server server.c)
target_link_libraries(server -lpthread)
