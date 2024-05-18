set(target_name core)

file(GLOB CORE_SRC
     "*.cpp"
)

add_library(${target_name}
  ${CORE_SRC}
)

include_directories(${CMAKE_SOURCE_DIR}/include/core)
