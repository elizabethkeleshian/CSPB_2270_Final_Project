#include "scene_graph/types.h"

namespace scene_graph {

float wrapAngle(float angle) {
    float shifted_angle = angle + glm::pi<float>();
    float wrapped_angle = glm::mod(shifted_angle, 2.0f * glm::pi<float>());
    float result = wrapped_angle - glm::pi<float>();
    return result;
}

} // namespace scene_graph 