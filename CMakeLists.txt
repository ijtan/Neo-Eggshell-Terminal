cmake_minimum_required(VERSION 3.16)
project(neoEggshell)

set(CMAKE_CXX_STANDARD 20)
add_executable(neoEggshell main.cpp ext/linenoise.c Executor.cpp parser.cpp internal.cpp variableHandler.cpp redirection.cpp signalHandler.cpp)