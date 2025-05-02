#include "scene_graph/types.h"

namespace scene_graph {

float wrapAngle(float angle) {
    // Handle missing or infinite angles 
    if (!isfinite(angle)) {
        return isnan(angle) ? angle : 0.0f;
    }

    // Get the  modulo in range [-2π, 2π]
    float result = std::fmod(angle, 2.0f * glm::pi<float>());
    
    // put in range [-π, π]
     if (result > glm::pi<float>()) {
        result -= 2.0f * glm::pi<float>();
    } else if (result < -glm::pi<float>()) {
        result += 2.0f * glm::pi<float>();
    }

    return result;
}

} // namespace scene_graph 