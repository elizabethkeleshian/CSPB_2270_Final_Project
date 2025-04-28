# Add test executable
add_executable(scene_graphs_tests
    scene_graph/node_test.cpp
    scene_graph/transform_test.cpp
    visualization/canvas_test.cpp
    visualization/tree_view_test.cpp
)

# Link test executable with libraries
target_link_libraries(scene_graphs_tests
    scene_graphs_core
    visualization_core
    ${GTEST_LIBRARIES}
    pthread
)

# Add test
add_test(NAME scene_graphs_tests COMMAND scene_graphs_tests) 