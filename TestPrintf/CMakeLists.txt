cmake_minimum_required(VERSION 3.16)
project(testPrintf C)

set(CMAKE_C_COMPILER gcc)

set(CMAKE_CXX_COMPILER gcc)

add_compile_options(-std=gnu99 -Wall  -Werror)

add_executable(testPrintf main.c)