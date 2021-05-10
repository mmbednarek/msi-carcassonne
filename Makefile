.PHONY: build deps

deps:
	echo dependencies

build:
	mkdir -p build
	cmake -B build .
	cmake --build build

test:
	./build/src/Test/msi_test