project(msi_carcassonne_bin)
add_executable(msi_carcassonne_bin main.cpp)
target_link_libraries(msi_carcassonne_bin fmt msi_carcassonne_frontend msi_carcassonne_game msi_carcassonne_ai caffe glog protobuf)
target_include_directories(msi_carcassonne_bin PUBLIC "${PROJECT_SOURCE_DIR}/proto")

configure_file(../../../proto/model.prototxt model.prototxt COPYONLY)
configure_file(../../../proto/solver.prototxt solver.prototxt COPYONLY)
