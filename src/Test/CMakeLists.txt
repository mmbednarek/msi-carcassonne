project(msi_test)
add_executable(msi_test TileTest.cpp)
target_link_libraries(msi_test PUBLIC gtest gtest_main libmb)
