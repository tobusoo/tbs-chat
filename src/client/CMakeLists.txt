set(target_name tbs-client)

add_executable(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    client.cpp
)

target_link_libraries(
    ${target_name}
    core
)

include_directories(${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CMAKE_SOURCE_DIR}/include)