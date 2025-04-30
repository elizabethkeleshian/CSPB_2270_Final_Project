# Add test executable
add_executable(scene_graphs_tests
    main_test.cpp
    scene_graph/transform_test.cpp
    scene_graph/types_test.cpp
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