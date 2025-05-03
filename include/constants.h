// include/constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
// Window settings
inline constexpr int DEFAULT_WINDOW_WIDTH = 800;
inline constexpr int DEFAULT_WINDOW_HEIGHT = 600;
inline constexpr float ASPECT_RATIO = static_cast<float>(DEFAULT_WINDOW_WIDTH) /
                                      static_cast<float>(DEFAULT_WINDOW_HEIGHT);
inline constexpr const char *WINDOW_TITLE = "Scene Graph Visualization";

// Scene view settings
inline constexpr float SCENE_WIDTH = 20.0F;
inline constexpr float SCENE_HEIGHT = SCENE_WIDTH / ASPECT_RATIO;
inline constexpr float SCENE_HALF_WIDTH = SCENE_WIDTH / 2.0F;
inline constexpr float SCENE_HALF_HEIGHT = SCENE_HEIGHT / 2.0F;

// Car settings
inline constexpr float CAR_BODY_WIDTH = 4.0F;
inline constexpr float CAR_BODY_HEIGHT = 1.5F;
inline constexpr float CAR_WHEEL_RADIUS = 0.6F;
inline constexpr float CAR_WHEEL_OFFSET_X =
    1.5F; // Distance from center to wheels
inline constexpr float CAR_WHEEL_OFFSET_Y = -0.5F; // Height offset for wheels

// Car animation
inline constexpr float CAR_ROTATION_SPEED = 30.0F;    // degrees per second
inline constexpr float WHEEL_ROTATION_SPEED = 180.0F; // degrees per second
inline constexpr float CAR_MOVEMENT_SPEED = 2.0F;     // units per second
inline constexpr float WHEEL_ROTATION_FACTOR =
    90.0F; // Rotation factor for wheels during drag
inline constexpr float RED_CAR_CIRCLE_RADIUS =
    4.0F; // Radius of red car's circular path
inline constexpr float RED_CAR_MOVEMENT_SPEED =
    0.5F; // Speed of red car's circular movement
inline constexpr float BLUE_CAR_OSCILLATION_CENTER =
    3.0F; // Center position for blue car's movement
inline constexpr float BLUE_CAR_OSCILLATION_AMPLITUDE =
    3.0F; // Amplitude of blue car's back-and-forth movement
inline constexpr float CAR_ROTATION_ADJUSTMENT =
    90.0F; // Adjustment for car rotation to face movement direction

// Shape defaults
inline constexpr float DEFAULT_CIRCLE_RADIUS = 0.5F;

// Renderer settings
inline constexpr int CIRCLE_SEGMENTS = 32;
inline constexpr int DEFAULT_VIEWPORT_WIDTH = 800;
inline constexpr int DEFAULT_VIEWPORT_HEIGHT = 600;

// Tree view settings
inline constexpr int TREE_VIEW_INDENT_SIZE = 20;   // Pixels per depth level
inline constexpr int TREE_VIEW_NODE_HEIGHT = 12;   // Height of a node in pixels
inline constexpr float TREE_VIEW_X_OFFSET = -9.5F; // Starting X position
inline constexpr float TREE_VIEW_Y_OFFSET = 9.0F;  // Starting Y position
inline constexpr float TREE_VIEW_X_SCALE = 20.0F;  // X scaling factor
inline constexpr float TREE_VIEW_Y_SCALE = 15.0F;  // Y scaling factor
inline constexpr float TREE_VIEW_WIDTH =
    SCENE_WIDTH * 0.25F; // Width of tree view area
inline constexpr int TREE_VIEW_STARTING_Y =
    20; // Initial Y position for tree view
inline constexpr float TREE_VIEW_SCREEN_X_FACTOR =
    50.0F; // Screen to scene X scale factor
inline constexpr float TREE_VIEW_SCREEN_Y_FACTOR =
    35.0F; // Screen to scene Y scale factor
inline constexpr float TREE_VIEW_SCREEN_X_OFFSET =
    9.0F; // Screen to scene X offset
inline constexpr float TREE_VIEW_SCREEN_Y_OFFSET =
    8.0F; // Screen to scene Y offset
inline constexpr float TREE_VIEW_NODE_WIDTH_BASE =
    150.0F; // Base width for tree nodes
inline constexpr float TREE_VIEW_NODE_WIDTH_ADJUST =
    10.0F; // Width adjustment factor

// Text rendering settings
inline constexpr int TEXT_FONT_SIZE = 40;  // Font size in pixels
inline constexpr float TEXT_SCALE = 0.01F; // Text scaling factor

// UI colors
inline constexpr float UI_SELECTED_COLOR[4] = {0.3F, 0.6F, 1.0F,
                                               1.0F}; // Blue for selected
inline constexpr float UI_NORMAL_COLOR[4] = {0.8F, 0.8F, 0.8F,
                                             1.0F}; // White for normal

// Starting positions for cars
inline constexpr float RED_CAR_START_X = -6.0F;
inline constexpr float RED_CAR_START_Y = 3.0F;
inline constexpr float BLUE_CAR_START_X = 6.0F;
inline constexpr float BLUE_CAR_START_Y = -3.0F;

// Coordinate conversion
inline constexpr float WINDOW_TO_SCENE_SCALE = 20.0F;
inline constexpr float WINDOW_TO_SCENE_OFFSET_X = 10.0F;
inline constexpr float WINDOW_TO_SCENE_OFFSET_Y = 10.0F;

} // namespace constants

#endif // CONSTANTS_H
