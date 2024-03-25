set(target_name tbs-chat)

add_executable(${target_name})
add_executable(test)

target_sources(
    test
    PRIVATE
    test.cpp
)


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

target_link_libraries(
    test
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)


include_directories(${CMAKE_SOURCE_DIR}/include)