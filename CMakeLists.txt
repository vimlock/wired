cmake_minimum_required(VERSION 3.10)

project(wired VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")

add_subdirectory(lib)
