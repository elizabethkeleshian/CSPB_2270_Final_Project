// include/constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @file constants.h
 * @brief Central file for application constants
 *
 * This file contains all constants used throughout the application,
 * organized into namespaces.
 */
namespace constants {

/**
 * @brief UI and visualization color constants
 */
namespace colors {
// Background colors
/// Main application background color (dark blue-gray)
inline constexpr float BACKGROUND[4] = {0.15f, 0.18f, 0.22f, 1.0f};
/// Tree view background color (slightly darker than main background)
inline constexpr float TREE_VIEW_BACKGROUND[4] = {0.12f, 0.14f, 0.17f, 1.0f};
/// Renderer clear color
inline constexpr float RENDERER_CLEAR[4] = {0.2f, 0.3f, 0.3f, 1.0f};

// Primary accent colors
/// Primary accent (vibrant blue)
inline constexpr float PRIMARY[4] = {0.0f, 0.47f, 0.9f, 1.0f};
/// Secondary accent (emerald green)
inline constexpr float SECONDARY[4] = {0.0f, 0.73f, 0.52f, 1.0f};
/// Highlight (amber)
inline constexpr float HIGHLIGHT[4] = {1.0f, 0.76f, 0.03f, 1.0f};
/// Text (off-white)
inline constexpr float TEXT[4] = {0.93f, 0.94f, 0.95f, 1.0f};
/// Alert (coral red)
inline constexpr float ALERT[4] = {0.95f, 0.33f, 0.33f, 1.0f};

// UI element colors
/// Highlighted selection color (blue)
inline constexpr float UI_SELECTED[4] = {0.3f, 0.6f, 1.0f, 1.0f};
/// Normal UI element color
inline constexpr float UI_NORMAL[4] = {0.2f, 0.22f, 0.25f, 1.0f};
/// UI Selected text color
inline constexpr float UI_SELECTED_TEXT[4] = {1.0f, 1.0f, 1.0f, 1.0f};
/// UI Normal text color
inline constexpr float UI_NORMAL_TEXT[4] = {0.9f, 0.9f, 0.9f, 0.9f};
/// UI line color for tree view connections
inline constexpr float UI_LINE[4] = {0.6f, 0.6f, 0.6f, 0.8f};
/// UI separator line color
inline constexpr float UI_SEPARATOR[4] = {0.4f, 0.4f, 0.5f, 0.8f};
/// Title text color
inline constexpr float TITLE_TEXT[4] = {1.0f, 1.0f, 1.0f, 1.0f};
/// Selected node highlight color
inline constexpr float NODE_SELECTED[4] = {1.0f, 1.0f, 0.0f, 1.0f};

// Car colors
/// Red car body color
inline constexpr float RED_CAR[4] = {0.8f, 0.2f, 0.2f, 1.0f};
/// Blue car body color
inline constexpr float BLUE_CAR[4] = {0.2f, 0.4f, 0.8f, 1.0f};
/// Car wheel color (dark gray/black)
inline constexpr float CAR_WHEEL[4] = {0.2f, 0.2f, 0.2f, 1.0f};
/// Car hubcap color (silver)
inline constexpr float CAR_HUBCAP[4] = {0.8f, 0.8f, 0.8f, 1.0f};
}  // namespace colors

/**
 * @brief Window and viewport settings
 */
/// Default window width in pixels
inline constexpr int DEFAULT_WINDOW_WIDTH = 800;
/// Default window height in pixels
inline constexpr int DEFAULT_WINDOW_HEIGHT = 600;
/// Window aspect ratio
inline constexpr float ASPECT_RATIO =
    static_cast<float>(DEFAULT_WINDOW_WIDTH) / static_cast<float>(DEFAULT_WINDOW_HEIGHT);
/// Window title
inline constexpr const char* WINDOW_TITLE = "Scene Graph Visualization";

/**
 * @brief Scene view settings
 */
/// Width of the scene in world units
inline constexpr float SCENE_WIDTH = 20.0f;
/// Height of the scene in world units
inline constexpr float SCENE_HEIGHT = SCENE_WIDTH / ASPECT_RATIO;
/// Half of the scene width
inline constexpr float SCENE_HALF_WIDTH = SCENE_WIDTH / 2.0f;
/// Half of the scene height
inline constexpr float SCENE_HALF_HEIGHT = SCENE_HEIGHT / 2.0f;

/**
 * @brief Car visual settings
 */
/// Width of car body
inline constexpr float CAR_BODY_WIDTH = 4.0f;
/// Height of car body
inline constexpr float CAR_BODY_HEIGHT = 1.5f;
/// Radius of car wheels
inline constexpr float CAR_WHEEL_RADIUS = 0.6f;
/// Horizontal distance from car center to wheels
inline constexpr float CAR_WHEEL_OFFSET_X = 1.5f;
/// Vertical offset for wheels from car body center
inline constexpr float CAR_WHEEL_OFFSET_Y = -0.5f;
/// Car roof width factor (relative to body width)
inline constexpr float CAR_ROOF_WIDTH_FACTOR = 0.5f;
/// Car roof height factor (relative to body height)
inline constexpr float CAR_ROOF_HEIGHT_FACTOR = 0.8f;
/// Car roof vertical position factor (relative to body height)
inline constexpr float CAR_ROOF_POSITION_FACTOR = 0.7f;
/// Hubcap radius factor (relative to wheel radius)
inline constexpr float CAR_HUBCAP_RADIUS_FACTOR = 0.4f;

/**
 * @brief Car animation settings
 */
/// Car rotation speed in degrees per second
inline constexpr float CAR_ROTATION_SPEED = 30.0f;
/// Wheel rotation speed in degrees per second
inline constexpr float WHEEL_ROTATION_SPEED = 180.0f;
/// Car movement speed in units per second
inline constexpr float CAR_MOVEMENT_SPEED = 2.0f;
/// Rotation factor for wheels during drag
inline constexpr float WHEEL_ROTATION_FACTOR = 90.0f;
/// Radius of red car's circular path
inline constexpr float RED_CAR_CIRCLE_RADIUS = 4.0f;
/// Speed of red car's circular movement
inline constexpr float RED_CAR_MOVEMENT_SPEED = 0.5f;
/// Center position for blue car's movement
inline constexpr float BLUE_CAR_OSCILLATION_CENTER = 3.0f;
/// Amplitude of blue car's back-and-forth movement
inline constexpr float BLUE_CAR_OSCILLATION_AMPLITUDE = 3.0f;
/// Adjustment for car rotation to face movement direction
inline constexpr float CAR_ROTATION_ADJUSTMENT = 90.0f;

/**
 * @brief Shape defaults
 */
/// Default circle radius
inline constexpr float DEFAULT_CIRCLE_RADIUS = 0.5f;

/**
 * @brief Renderer settings
 */
/// Number of segments used to draw circles
inline constexpr int CIRCLE_SEGMENTS = 32;
/// Default line thickness
inline constexpr float DEFAULT_LINE_THICKNESS = 0.02f;

/**
 * @brief Tree view layout settings
 */
/// Starting Y position in scene coordinates
inline constexpr float TREE_VIEW_Y_OFFSET = 3.0f;
/// Tree view indentation size in scene units
inline constexpr float TREE_INDENT_SIZE = 0.2f;
/// Vertical spacing between tree nodes in scene units
inline constexpr float TREE_VERT_SPACING = 0.8f;
/// Width of tree view area as proportion of scene width
inline constexpr float TREE_VIEW_WIDTH = SCENE_WIDTH * 0.3f;
/// Initial Y position for tree view
inline constexpr float TREE_VIEW_STARTING_Y = 0.5F;
/// Screen to scene X offset
inline constexpr float TREE_VIEW_SCREEN_X_OFFSET = 9.0f;
/// Screen to scene Y offset
inline constexpr float TREE_VIEW_SCREEN_Y_OFFSET = 4.0f;
/// Tree view title Y offset from top
inline constexpr float TREE_VIEW_TITLE_OFFSET_Y = 0.02f;
/// Tree view separator Y offset from title
inline constexpr float TREE_VIEW_SEPARATOR_OFFSET = 0.02f;
/// Tree view title padding
inline constexpr float TREE_VIEW_TITLE_PADDING = 0.1f;
/// Tree view separator padding
inline constexpr float TREE_VIEW_SEPARATOR_PADDING = 0.15f;
/// Space between elements in tree view
inline constexpr float TREE_VIEW_ELEMENT_PADDING = 0.2f;
/// Node vertical centering offset
inline constexpr float TREE_NODE_VERTICAL_OFFSET = 0.15f;
/// Node horizontal connector padding
inline constexpr float TREE_NODE_CONNECTOR_PADDING = 0.1f;

/**
 * @brief Text rendering settings
 */
/// Font size for text
inline constexpr int TEXT_FONT_SIZE = 24;
/// Global scale for text rendering
inline constexpr float TEXT_SCALE = 0.012f;
/// Horizontal padding for text
inline constexpr float TEXT_PADDING_X = 0.3f;
/// Vertical padding for text
inline constexpr float TEXT_PADDING_Y = 0.1f;
/// Height for tree nodes
inline constexpr float TREE_NODE_HEIGHT = 0.7f;
/// Vertical offset to center text in tree nodes
inline constexpr float TREE_TEXT_VERT_OFFSET = 0.1f;
/// Character width scaling factor for text
inline constexpr float TEXT_CHAR_WIDTH_FACTOR = 0.25f;
/// Fixed padding for text width calculation
inline constexpr float TEXT_WIDTH_PADDING = 0.6f;

/**
 * @brief UI selection colors
 */
/// Color for selected UI elements
inline constexpr float UI_SELECTED_COLOR[4] = {0.3f, 0.6f, 1.0f, 1.0f};
/// Color for normal UI elements
inline constexpr float UI_NORMAL_COLOR[4] = {0.8f, 0.8f, 0.8f, 1.0f};

/**
 * @brief Starting positions for cars
 */
/// Initial X position for red car
inline constexpr float RED_CAR_START_X = -1.0F;
/// Initial Y position for red car
inline constexpr float RED_CAR_START_Y = 5.0F;
/// Initial X position for blue car
inline constexpr float BLUE_CAR_START_X = 6.0F;
/// Initial Y position for blue car
inline constexpr float BLUE_CAR_START_Y = -5.0F;

/**
 * @brief Coordinate conversion constants
 */
/// Scale factor for window to scene conversion
inline constexpr float WINDOW_TO_SCENE_SCALE = 20.0F;
/// X offset for window to scene conversion
inline constexpr float WINDOW_TO_SCENE_OFFSET_X = 10.0F;
/// Y offset for window to scene conversion
inline constexpr float WINDOW_TO_SCENE_OFFSET_Y = 10.0F;

}  // namespace constants

#endif  // CONSTANTS_H
