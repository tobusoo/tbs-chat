set(target_name tbs-client)

add_executable(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    client.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)