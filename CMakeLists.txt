cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# filesystem.h require C++17
set(CMAKE_CXX_STANDARD 17)

file(COPY sample_data/ DESTINATION sample_data/)

# show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

#add_executable(rapidJSONExample rapidJSONExample.cpp)
add_executable(test test.cpp)
add_executable(supersearch supersearch.cpp)

set(CMAKE_VERBOSE_MAKEFILE OFF)
