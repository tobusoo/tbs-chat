set(target_name tbs-chat)
add_executable(${target_name})

file(GLOB SRC
     "*.cpp"
)

target_sources(
    ${target_name}
    PRIVATE
    ${SRC}
)

set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 20)

target_link_libraries(
    ${target_name}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE core
    PRIVATE ui
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/core)
include_directories(${CMAKE_SOURCE_DIR}/external)
