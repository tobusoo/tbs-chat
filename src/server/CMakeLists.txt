set(target_name tbs-server)

add_executable(${target_name})

file(GLOB SRC
     "*.cpp"
)

target_sources(
    ${target_name}
    PRIVATE
    ${SRC}
)

target_link_libraries(
    ${target_name}
    core
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/core)
include_directories(${CMAKE_SOURCE_DIR}/external)