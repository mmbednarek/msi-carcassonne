add_executable(carcassonne_headless main.cpp)
target_link_libraries(carcassonne_headless msi_carcassonne_game msi_carcassonne_ai fmt Boost::program_options spdlog)

configure_file(../../../proto/model.prototxt model.prototxt COPYONLY)
configure_file(../../../proto/net_tic_tac_6_4_2_res_block.prototxt net_tic_tac_6_4_2_res_block.prototxt COPYONLY)
configure_file(../../../proto/solver.prototxt solver.prototxt COPYONLY)