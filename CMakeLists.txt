cmake_minimum_required(VERSION 3.22)

project(
    tbs-chat
    VERSION 1.0
    LANGUAGES CXX   
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(external)
