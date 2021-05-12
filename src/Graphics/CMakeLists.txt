project(msi_graphics)
add_library(msi_graphics Surface.cpp Context.cpp Texture.cpp TextCache.cpp Font.cpp)
target_link_libraries(msi_graphics PUBLIC SDL2 SDL2_image SDL2_ttf libmb fmt)
