cmake_minimum_required(VERSION 3.30)
project(Chat C)

add_subdirectory("${PROJECT_SOURCE_DIR}/ChatServer/" "${PROJECT_SOURCE_DIR}/ChatServer/cmake-build-debug/")
add_subdirectory("${PROJECT_SOURCE_DIR}/Client/" "${PROJECT_SOURCE_DIR}/Client/cmake-build-debug")
