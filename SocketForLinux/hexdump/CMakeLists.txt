cmake_minimum_required(VERSION 2.8)
project(hexdump)

set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/../include)

aux_source_directory(. DIR_LIB_SRCS)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)

add_library(${PROJECT_NAME} SHARED ${DIR_LIB_SRCS})
