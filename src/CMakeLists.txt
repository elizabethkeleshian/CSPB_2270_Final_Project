# add application library
add_library(application_core
    application.cpp
)

# Add scene graph library
add_library(scene_graphs_core
    scene_graph/transform.cpp
    scene_graph/types.cpp
    scene_graph/node.cpp
    scene_graph/shape.cpp
    scene_graph/circle.cpp
    scene_graph/rectangle.cpp
)

# Add visualization library
add_library(visualization_core
    visualization/canvas.cpp
    visualization/tree_view.cpp
    visualization/shader.cpp
    visualization/renderer.cpp
    visualization/window.cpp
    visualization/shape_renderer.cpp
    visualization/text_renderer.cpp
    visualization/font_manager.cpp
    visualization/shader_manager.cpp
    visualization/render_types.cpp
)

# Include directories
target_include_directories(scene_graphs_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(visualization_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Find FreeType package
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Link OpenGL libraries to visualization_core
target_link_libraries(visualization_core
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    glfw
    ${FREETYPE_LIBRARIES}
)

# Link GLM to bring in its headers
target_link_libraries(scene_graphs_core PUBLIC glm::glm) 