.PHONY: all checkout setup build test

build:
	mkdir -p build
	cd build && cmake ..
	cd build && cmake --build .

test:
	cd build && ctest

clean:
	rm -rf build