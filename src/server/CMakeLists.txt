set(target_name tbs-server)

add_executable(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    server.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)