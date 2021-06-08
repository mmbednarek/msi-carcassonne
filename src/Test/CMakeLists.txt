project(msi_test)
add_executable(msi_test TileTest.cpp UtilTest.cpp BoardTest.cpp EvolutionTest.cpp)
target_link_libraries(msi_test PUBLIC gtest gtest_main libmb msi_carcassonne_game msi_evolution fmt)
