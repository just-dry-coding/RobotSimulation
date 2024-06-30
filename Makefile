.PHONY: all checkout setup build test

run:
	cd build && ./RobotSimulator

build:
	mkdir -p build
	cd build && cmake ..
	cd build && cmake --build .

test:
	cd build && ctest --output-on-failure

clean:
	rm -rf build