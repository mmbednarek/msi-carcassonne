CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 cmake -S . -B out/build
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 make -C out/build -j12
./out/build/src/bin/carcassonne/msi_carcassonne_bin
