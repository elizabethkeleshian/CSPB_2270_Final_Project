# Add scene graph library
add_library(scene_graphs_core
    scene_graph/node.cpp
    scene_graph/transform.cpp
)

# Add visualization library
add_library(visualization_core
    visualization/canvas.cpp
    visualization/tree_view.cpp
)

# Include directories
target_include_directories(scene_graphs_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(visualization_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
) 