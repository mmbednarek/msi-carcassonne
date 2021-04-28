project(msi_input)
add_library(msi_input Input.cpp)
target_link_libraries(msi_input PUBLIC SDL2 libmb)
