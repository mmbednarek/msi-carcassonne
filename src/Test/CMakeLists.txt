project(msi_test)
add_executable(msi_test TileTest.cpp UtilTest.cpp BoardTest.cpp)
target_link_libraries(msi_test PUBLIC gtest gtest_main libmb msi_carcassonne_game msi_carcassonne_ai fmt)

project(caffe_test)
add_executable(caffe_test CaffeTest.cpp)
target_link_libraries(caffe_test PUBLIC gtest gtest_main fmt caffe glog protobuf)
target_include_directories(caffe_test PUBLIC "${PROJECT_SOURCE_DIR}/proto")

configure_file(../../proto/model.prototxt model.prototxt COPYONLY)
configure_file(../../proto/net_tic_tac_6_4_2_res_block.prototxt net_tic_tac_6_4_2_res_block.prototxt COPYONLY)
configure_file(../../proto/solver.prototxt solver.prototxt COPYONLY)
