project(msi_carcassonne_graphics)
add_library(msi_carcassonne_graphics Surface.cpp SDLContext.cpp Texture.cpp Resource.cpp)
target_link_libraries(msi_carcassonne_graphics PUBLIC SDL2 SDL2_image libmb)
