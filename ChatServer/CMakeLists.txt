cmake_minimum_required(VERSION 3.30)
project(ChatServer C)

set(CMAKE_C_STANDARD 99)

add_executable(ChatServer main.c
        main.h
        commands.c
        commands.h)
target_link_libraries(ChatServer Ws2_32.lib)
