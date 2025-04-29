.PHONY: all clean build test docs

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

# Generate documentation
docs:
	doxygen Doxyfile

# Clean build files
clean:
	rm -rf build 