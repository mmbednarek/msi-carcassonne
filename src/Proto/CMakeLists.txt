include(api.cmake)
add_library(carcassonne_proto ${proto_sources})
target_link_libraries(carcassonne_proto protobuf)