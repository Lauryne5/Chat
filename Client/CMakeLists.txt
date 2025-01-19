cmake_minimum_required(VERSION 3.30)
project(Client C)

set(CMAKE_C_STANDARD 99)

add_executable(Client main.c)
target_link_libraries(Client Ws2_32.lib)
