cmake_minimum_required(VERSION 3.10)
project(SceneGraphs VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add subdirectories first to define libraries
add_subdirectory(src)
add_subdirectory(test)

# Add executable for main application
add_executable(scene_graphs_app src/main.cpp)

# Link libraries
target_link_libraries(scene_graphs_app 
    scene_graphs_core
    visualization_core
)

# Enable testing
enable_testing()

# Add test executable
add_executable(scene_graphs_test_executable 
    test/scene_graph/node_test.cpp
    test/scene_graph/transform_test.cpp
    test/scene_graph/types_test.cpp
)

# Link test executable with core library and GTest
target_link_libraries(scene_graphs_test_executable 
    scene_graphs_core
    visualization_core
    ${GTEST_LIBRARIES}
    pthread
)

# Add test
add_test(NAME scene_graphs_tests COMMAND scene_graphs_test_executable) 