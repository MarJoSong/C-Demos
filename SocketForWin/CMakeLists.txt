cmake_minimum_required(VERSION 3.16)
project(SocketForWin)

set(CMAKE_CXX_STANDARD 14)

add_executable(SocketForWin main.cpp)

target_link_libraries(${PROJECT_NAME} Ntdll.dll)