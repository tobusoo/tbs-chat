set(target_name tbs-chat)

add_executable(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    main.cpp
)

target_link_libraries(
    ${target_name}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)

include_directories(${CMAKE_SOURCE_DIR}/include)