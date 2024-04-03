set(target_name tbs-server)

add_executable(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    server.cpp
)

target_link_libraries(
    ${target_name}
    core
)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)