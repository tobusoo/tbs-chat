set(target_name core)

add_library(${target_name}
  Package.pb.cc
)

target_link_libraries(
  ${target_name}
  PUBLIC
  protobuf
)
