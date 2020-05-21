cmake_minimum_required(VERSION 3.16)
project(neoEggshell)

set(CMAKE_CXX_STANDARD 20)

add_executable(neoEggshell ext/linenoise.c external.cpp parser.cpp internal.cpp main.cpp variableHandler.cpp rediraction.cpp)