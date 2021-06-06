cmake -D CMAKE_C_COMPILER=gcc-10 -D CMAKE_CXX_COMPILER=g++-10 -S . -B out/build_terminal
make -C out/build_terminal -j12 && ./out/build_terminal/src/bin/carcassonne/msi_carcassonne_bin
