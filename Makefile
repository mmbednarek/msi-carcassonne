.PHONY: build deps

deps:
	echo dependencies

build:
	mkdir -p build && cd build
	cmake .. && cmake --build .