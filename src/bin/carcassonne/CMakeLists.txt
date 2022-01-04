project(msi_carcassonne_bin)
add_executable(msi_carcassonne_bin main.cpp)
target_link_libraries(msi_carcassonne_bin fmt msi_carcassonne_frontend msi_carcassonne_game msi_carcassonne_ai)

# configure_file(../../../resource/font.ttf resource/font.ttf COPYONLY)
# configure_file(../../../resource/figures.png resource/figures.png COPYONLY)
# configure_file(../../../resource/tiles.png resource/tiles.png COPYONLY)
# configure_file(../../../resource/yellow.png resource/yellow.png COPYONLY)