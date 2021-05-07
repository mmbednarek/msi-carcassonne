export PATH=cmake-install/bin:$PATH
export CMAKE_PREFIX_PATH=cmake-install:$CMAKE_PREFIX_PATH
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 cmake -S . -B out/build
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 make -C out/build
./out/build/src/bin/carcassonne/msi_carcassonne_bin
