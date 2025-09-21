cmake_minimum_required(VERSION 3.15)
project(minez C)

set(CMAKE_C_STANDARD 11)

add_executable(minez
    src/main.c
    src/interpreter.c
    src/vectors.c
)

target_include_directories(minez PRIVATE include)