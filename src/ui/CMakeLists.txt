set(target_name ui)

file(GLOB SRC
     "*.cpp"
)

add_library(${target_name}
  ${SRC}
)

target_link_libraries(
  ${target_name}
  PUBLIC
  ftxui::screen
  ftxui::dom
  ftxui::component
)

include_directories(${CMAKE_SOURCE_DIR}/include)