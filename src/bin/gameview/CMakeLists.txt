add_executable(carcassonne_gameview main.cpp)
target_link_libraries(carcassonne_gameview fmt msi_carcassonne_frontend msi_carcassonne_game msi_carcassonne_ai pthread carcassonne_proto carcassonne_encoder)

# configure_file(../../../resource/font.ttf resource/font.ttf COPYONLY)
# configure_file(../../../resource/figures.png resource/figures.png COPYONLY)
# configure_file(../../../resource/tiles.png resource/tiles.png COPYONLY)
# configure_file(../../../resource/yellow.png resource/yellow.png COPYONLY)