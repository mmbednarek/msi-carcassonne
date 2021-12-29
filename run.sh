cmake -D CMAKE_C_COMPILER=gcc-10 -D CMAKE_CXX_COMPILER=g++-10 -S . -B out/build_terminal
# make -C out/build_terminal -j12 && ./out/build_terminal/src/bin/evolution/msi_evolution_bin
make -C out/build_terminal -j${nproc} && 
# ./out/build_terminal/src/bin/carcassonne/msi_carcassonne_bin
./out/build_terminal/src/bin/headless/carcassonne_headless -v -s 456
# ./out/build_terminal/src/bin//headless_gpu/carcassonne_gpu_headless -v -s 456
cd ./out/build_terminal/src/Test/
./caffe_test
cd -
