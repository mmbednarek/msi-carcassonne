export PATH=cmake-install/bin:$PATH
export CMAKE_PREFIX_PATH=cmake-install:$CMAKE_PREFIX_PATH
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 cmake -S . -B out/build
CXX=/usr/bin/g++-10 CC=/usr/bin/gcc-10 make -C out/build
# ./out/build/src/Test/MSI_Test
# ./out/build/src/Tool/cvrp-evo/MSI_Tool_Evo src/CVRP/E-n51-k5_VERT_COORD.txt ./src/CVRP/E-n51-k5_DEMANDS.txt
