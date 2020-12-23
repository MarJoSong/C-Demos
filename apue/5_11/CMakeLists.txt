# cmake_minimum_required(VERSION <specify CMake version here>)
project(5_11)

set(CMAKE_C_STANDARD 11)

add_executable(5_11 main.c)
target_link_libraries(${PROJECT_NAME} -lapue)