// include/constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
// Window settings
inline constexpr int DEFAULT_WINDOW_WIDTH = 800;
inline constexpr int DEFAULT_WINDOW_HEIGHT = 600;
inline constexpr float ASPECT_RATIO = static_cast<float>(DEFAULT_WINDOW_WIDTH) /
                                      static_cast<float>(DEFAULT_WINDOW_HEIGHT);
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

// Shape defaults
inline constexpr float DEFAULT_CIRCLE_RADIUS = 0.5F;

// Renderer settings
inline constexpr int CIRCLE_SEGMENTS = 32;

} // namespace constants

#endif // CONSTANTS_H
