cmake_minimum_required(VERSION 3.10)
project(SceneGraphs VERSION 1.0)

# Add this line to generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)

# Set explicit paths for GLEW
set(GLEW_INCLUDE_DIR "/usr/local/include")
set(GLEW_LIBRARY "/usr/local/lib/libGLEW.dylib")
set(GLEW_LIBRARIES ${GLEW_LIBRARY})

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

# Include directories
include_directories(include)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})

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
    application_core
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    glfw
    ${FREETYPE_LIBRARIES}
)

# Enable testing
enable_testing() 