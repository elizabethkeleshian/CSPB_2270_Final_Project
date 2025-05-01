# Add test executable
add_executable(scene_graphs_tests
    main_test.cpp
    scene_graph/transform_test.cpp
    scene_graph/types_test.cpp
    scene_graph/node_test.cpp
    # scene_graph/shape_test.cpp
    # scene_graph/rectangle_test.cpp
    # scene_graph/circle_test.cpp
)

# Link test executable with libraries
target_link_libraries(scene_graphs_tests
    scene_graphs_core
   # visualization_core
    ${GTEST_LIBRARIES}
    pthread
)

# Add tests
add_test(NAME transform_tests COMMAND scene_graphs_tests --gtest_filter=scene_graph::TransformTest*)
add_test(NAME types_tests COMMAND scene_graphs_tests --gtest_filter=scene_graph::TypesTest*)
add_test(NAME node_tests COMMAND scene_graphs_tests --gtest_filter=scene_graph::NodeTest*)
# add_test(NAME shape_tests COMMAND scene_graphs_tests --gtest_filter=scene_graph::ShapeTest*)
# add_test(NAME rectangle_tests COMMAND scene_graphs_tests --gtest_filter=scene_graph::RectangleTest*)
# add_test(NAME circle_tests COMMAND scene_graphs_tests --gtest_filter=scene_graph::CircleTest*) 