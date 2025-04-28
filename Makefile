.PHONY: all clean build test

# Default target
all: build

# Create build directory and run cmake
build:
	mkdir -p build
	cd build && cmake ..
	cd build && make

# Run tests
test: build
	cd build && ctest --output-on-failure

# Clean build files
clean:
	rm -rf build 