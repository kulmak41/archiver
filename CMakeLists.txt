cmake_minimum_required(VERSION 3.26)
project(archiver)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
