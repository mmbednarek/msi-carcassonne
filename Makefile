.PHONY: build deps

deps:
	wget https://github.com/Kitware/CMake/releases/download/v3.20.2/cmake-3.20.2-linux-x86_64.sh -O cmake.sh
	sudo sh cmake.sh --prefix=/usr/local/ --exclude-subdir

build:
	mkdir -p build
	/usr/local/bin/cmake -B build .
	/usr/local/bin/cmake --build build

test:
	./build/src/Test/msi_test