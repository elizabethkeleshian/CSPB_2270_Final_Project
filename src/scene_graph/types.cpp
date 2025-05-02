#include "scene_graph/types.h"

namespace scene_graph {

float wrapAngle(float angle) {
  // Handle missing or infinite angles
  if (!std::isfinite(angle)) {
    return std::isnan(angle) ? angle : 0.0F;
  }

  const float two_pi = 2.0F * glm::pi<float>();
  const auto pi = glm::pi<float>();

  // Get the  modulo in range [-2π, 2π]
  float result = std::fmod(angle, two_pi);

  // put in range [-π, π]
  if (result > pi) {
    result -= two_pi;
  } else if (result < -pi) {
    result += two_pi;
  }

  return result;
}

} // namespace scene_graph