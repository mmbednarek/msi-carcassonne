CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 cmake -S . -B out/build_terminal
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 make -C out/build_terminal -j12 && ./out/build_terminal/src/bin/carcassonne/msi_carcassonne_bin
